/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:54:50 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/17 17:24:02 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/*                               STANDARD LIBS                                */
/* ************************************************************************** */

# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdint.h>

/* ************************************************************************** */
/*                                CUSTOM LIBS                                 */
/* ************************************************************************** */

# include "../../libft/libft/libft.h"
# include "../../libft/gnl/get_next_line_bonus.h"

/* ************************************************************************** */
/*                                  READLINE                                  */
/* ************************************************************************** */

# include <readline/readline.h>
# include <readline/history.h>

/* ************************************************************************** */
/*                                  STRUCTS                                   */
/* ************************************************************************** */

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	int				argc;
	char			**argv;

	char			*input_file;
	char			*output_file;
	bool			append_out;
	bool			input_redirect;
	bool			output_redirect;

	int				input_fd;
	int				output_fd;
	bool			is_builtin;

	struct s_cmd	*next;
}	t_cmd;

typedef struct s_minishell
{
	t_cmd	*cmds;
	int		cmd_cnt;

	char	**env;
	char	**path;

	char	*hostname;
	char	*user;
	char	*home;

	int		exit_status;
	int		**pipes;
	int		pipe_cnt;
	int		hd_cnt;

	bool	heredoc_interrupted;
	bool	had_syntax_error;
}	t_msh;

typedef enum e_exit
{
	SUCCESS,
	FAILURE
}	t_exit;

/* ************************************************************************** */
/*                                  GLOBALS                                   */
/* ************************************************************************** */

extern int	g_exit;

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

# define NO_ENV			-1
# define NO_VAR			-1
# define INIT_ERR		"init error\n"
# define MALLOC_ERR		"malloc error\n"
# define ENV_INIT_ERR	"env init error\n"

/* ************************************************************************** */
/*                               INITIALIZATION                               */
/* ************************************************************************** */

int		init(t_msh *sh, char **env);
char	**init_env(char **env);

/* ************************************************************************** */
/*                                  SIGNALS                                   */
/* ************************************************************************** */

void	set_interactive_signals(void);
void	surpress_parent_sig(struct sigaction *o_int, struct sigaction *o_quit);
void	restore_parent_sig(struct sigaction *o_int, struct sigaction *o_quit);
void	setup_heredoc_signals(void);
void	reset_child_signals(void);

/* ************************************************************************** */
/*                                   PARSER                                   */
/* ************************************************************************** */

t_token	*parse_input(const char *line, t_msh *sh);
char	*expand_one(const char *str, int *i, t_msh *sh);
char	*expand_token_value(const char *value, t_msh *sh);
void	handle_single_quote(const char *str, int *i, char **res);
void	handle_double_quote(const char *str, int *i, t_msh *sh, char **res);
void	handle_dollar(const char *str, int *i, t_msh *sh, char **res);

/* ************************************************************************** */
/*                                 TOKENIZER                                  */
/* ************************************************************************** */

int		process_token(t_token **tokens, t_cmd **current, t_msh *sh);
int		check_syntax(t_token *tokens);
int		is_special_char(char c);
char	*extract_special_char(const char *str, int *i);
char	*unescape_token(const char *str);
t_token	*token_new(char *value, t_token_type type, t_msh *sh);
void	token_add_back(t_token **list, t_token *new_token);
char	**split_input(const char *str);

/* ************************************************************************** */
/*                                 EXECUTION                                  */
/* ************************************************************************** */

int		exec_ast(t_token *tokens, t_msh *sh);
int		exec_pipeline(t_cmd *cmds, t_msh *sh, int in_fd);
int		run_single_command(t_cmd **cmds, t_msh *sh, int *in_fd, pid_t *pid_out);
int		execute_all(t_cmd *cmds, t_msh *sh, int *in_fd, pid_t *pids);
int		prepare_pipe(int pipe_fd[2], int has_next);
int		fork_command(t_cmd *cmd, t_msh *sh, int *in_fd, int pipe_fd[2]);
void	handle_parent_cleanup(int *in_fd, int pipe_fd[2], int has_next);
void	wait_all_children(pid_t *pids, int count);

/* ************************************************************************** */
/*                               CHILD PROCESS                                */
/* ************************************************************************** */

void	exec_child(t_cmd *cmds, int in_fd, int pipe_fd[2], t_msh *sh);


/* ************************************************************************** */
/*                                  BUILTINS                                  */
/* ************************************************************************** */

int		run_builtin(t_cmd *cmd, t_msh *sh);
t_cmd	*build_cmd_list(t_token *tokens, t_msh *sh);
void	finalize_cmds(t_cmd *cmd);
int		cmd_cd(t_cmd *cmd, t_msh *sh);
int		cmd_echo(t_cmd *cmd);
int		cmd_env(t_cmd *cmd, t_msh *sh);
int		cmd_exit(t_cmd *cmd, t_msh *sh);
int		cmd_export(t_cmd *cmd, t_msh *sh);

/* ************************************************************************** */
/*                            COMMAND CONSTRUCTION                            */
/* ************************************************************************** */

t_cmd	*cmd_new(void);
t_cmd	*cmd_from_tokens(t_token *tokens, t_msh *sh);
char	*get_cmd_path(char *cmd, char **env);
int		add_arg(t_cmd *cmd, char *value);
t_cmd	*reverse_cmd_list(t_cmd *cmd);

/* ************************************************************************** */
/*                          REDIRECTIONS & HEREDOCS                           */
/* ************************************************************************** */

int		process_redirect(t_cmd *cmd, t_token *token, t_msh *sh);
void	setup_redirections(t_cmd *cmds, int in_fd, int pipe_fd[2]);
int		handle_redirect_in(t_cmd *cmd, t_token *file_tok);
int		handle_redirect_out(t_cmd *cmd, t_token *file_tok, bool append);
int		handle_redirect_heredoc(t_cmd *cmd, t_token *file_tok, t_msh *sh);
char	*write_heredoc_to_tmp(const char *delim, int index);

/* ************************************************************************** */
/*                                ENVIRONMENT                                 */
/* ************************************************************************** */

char	*extract_var(char *var, char **env);
int		set_var(char *var, char *val, char ***env);
char	**env_add_var(char **env, char *new_var);
void	sort_env(char **env);
char	**copy_env_array(char **env);

/* ************************************************************************** */
/*                                   ERRORS                                   */
/* ************************************************************************** */

int		exit_error(char *msg, int status);
void	print_heredoc_warning(const char *delim);

/* ************************************************************************** */
/*                                  CLEANUP                                   */
/* ************************************************************************** */

void	free_split(char **split);
void	free_cmd(t_cmd *cmd);
void	free_token_list(t_token *token);
void	free_env_array(char **env);
void	free_minishell(t_msh *sh);

/* ************************************************************************** */
/*                                   UTILS                                    */
/* ************************************************************************** */

char	*build_prompt(t_msh *sh);
bool	needs_pipe_continuation(const char *line);

#endif
