/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:02:36 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/15 02:06:30 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Standard Libs
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdint.h>

// Custom Libs
# include "../../libft/libft/libft.h"
# include "../../libft/gnl/get_next_line_bonus.h"

// Readline
# include <readline/readline.h>
# include <readline/history.h>

// Structs
typedef struct s_cmd
{
	int				argc;
	char			**argv;
	int				input_fd;
	int				output_fd;
	bool			is_builtin;
	struct s_cmd	*next;
}	t_cmd;

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

typedef struct s_minishell
{
	t_cmd	*cmds;
	int		cmd_cnt;
	char	**env;
	char	**path;
	char	*hostname;
	char	*user;
	int		exit_status;
	int		**pipes;
	int		pipe_cnt;
	int		hd_cnt;
	char	*home;
}	t_minishell;

typedef enum e_exit
{
	SUCCESS,
	FAILURE
}	t_exit;

// 🧠 Globals
extern int	g_exit;

// ⚙️ Macros
# define NO_ENV			-1
# define NO_VAR			-1
# define INIT_ERR		"Init Error\n"
# define MALLOC_ERR		"Malloc Error\n"
# define ENV_INIT_ERR	"Env Init Error\n"

//	Execution
int		exec_tokens(t_token *tokens, t_minishell *sh);
int		exec_pipeline(t_cmd *cmds, t_minishell *sh, int in_fd);
int		run_single_command(t_cmd **cmds, t_minishell *sh, int *in_fd,
			pid_t *pid_out);
int		prepare_pipe(int pipe_fd[2], int has_next);
int		fork_command(t_cmd *cmd, t_minishell *sh, int *in_fd, int pipe_fd[2]);
void	handle_parent_cleanup(int *in_fd, int pipe_fd[2], int has_next);
void	wait_all_children(pid_t *pids, int count);
int		execute_all(t_cmd *cmds, t_minishell *sh, int *in_fd, pid_t *pids);

//	Child Process
void	exec_child(t_cmd *cmds, int in_fd, int pipe_fd[2], t_minishell *sh);
void	surpress_parent_sig(struct sigaction *o_int, struct sigaction *o_quit);
void	restore_parent_sig(struct sigaction *o_int, struct sigaction *o_quit);

//	Builtins
int		run_builtin(t_cmd *cmd, t_minishell *sh);
int		cmd_cd(t_cmd *cmd, t_minishell *sh);
int		cmd_echo(t_cmd *cmd);
int		cmd_env(t_cmd *cmd, t_minishell *sh);
int		cmd_exit(t_cmd *cmd, t_minishell *sh);
int		cmd_export(t_cmd *cmd, t_minishell *sh);

//	Command Construction
t_cmd	*cmd_new(void);
t_cmd	*cmd_from_tokens(t_token *tokens);
char	*get_cmd_path(char *cmd, char **env);
int		add_arg(t_cmd *cmd, char *value);

//	Parsing & Tokens
t_token	*parse_input(const char *line, t_minishell *sh);
char	**split_input(const char *str);
int		process_token(t_token **tokens, t_cmd **current);
int		check_syntax(t_token *tokens);
t_token	*token_new(char *value, t_token_type type, t_minishell *sh);
void	token_add_back(t_token **list, t_token *new_token);
int		is_special_char(char c);
char	*extract_special_char(const char *str, int *i);
char	*unescape_token(const char *str);

//	Expansions
char	*expand_token_value(const char *value, t_minishell *sh);
char	*expand_one(const char *str, int *i, t_minishell *sh);
void	handle_single_quote(const char *str, int *i, char **res);
void	handle_double_quote(const char *str, int *i, t_minishell *sh,
			char **res);
void	handle_dollar(const char *str, int *i, t_minishell *sh, char **res);

//	Redirections & Heredocs
int		process_redirect(t_cmd *cmd, t_token *token);
void	setup_redirections(t_cmd *cmds, int in_fd, int pipe_fd[2]);
int		handle_redirect_heredoc(t_cmd *cmd, t_token *file_tok);
int		process_heredoc(const char *delim);

//	Env
char	*extract_var(char *var, char **env);
int		set_var(char *var, char *val, char ***env);
char	**env_add_var(char **env, char *new_var);
void	sort_env(char **env);
char	**copy_env_array(char **env);

//	Utils
char	*build_prompt(t_minishell *sh);
bool	needs_pipe_continuation(const char *line);

//	Errors
int		exit_error(char *msg, int status);
void	print_heredoc_warning(const char *delim);

//	Cleanup
void	free_split(char **split);
void	free_cmd(t_cmd *cmd);
void	free_token_list(t_token *token);
void	free_env_array(char **env);
void	free_minishell(t_minishell *sh);

//	Init
int		init(t_minishell *sh, char **env);
char	**init_env(char **env);

#endif
