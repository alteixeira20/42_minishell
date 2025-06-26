/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:54:50 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 16:22:20 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/*                               STANDARD LIBS                                */
/* ************************************************************************** */

# include <stdbool.h>
# include <stdlib.h>
# include <stddef.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdint.h>
# include <limits.h>

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

typedef enum e_syntax_status
{
	SYNTAX_OK,
	SYNTAX_INCOMPLETE,
	SYNTAX_ERROR
}	t_syntax;

typedef enum e_redirect_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type		type;
	char				*filename;
	bool				quoted;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_hdoc_tmpfile
{
	char					*path;
	struct s_hdoc_tmpfile	*next;
	struct s_hdoc_tmpfile	*prev;
}	t_hdoc_tmpfile;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	bool			expanded_empty;
	bool			quoted;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_cmd
{
	int				argc;
	char			**argv;

	t_redirect		*redirects;
	bool			redirect_failed;
	char			*redirect_failed_path;


	int				input_fd;
	int				output_fd;
	bool			is_builtin;
	bool			is_valid;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_minishell
{
	t_cmd	*cmds;
	int		cmd_cnt;

	t_token	*tokens;
	char	**env;
	char	**path;

	char	*hostname;
	char	*user;
	char	*home;

	pid_t			*pids;

	int		exit_status;
	int		**pipes;
	int		pipe_cnt;
	int		hd_cnt;

	t_hdoc_tmpfile	*heredoc_tmpfiles;
	bool	heredoc_quoted;
	bool	heredoc_found;
	bool	is_heredoc;
	bool	heredoc_interrupted;
	bool	error_printed;
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

int			init_sh(t_msh *sh, char **env);
char		**init_env(char **env);
t_token		*init_token(void);
t_msh		*get_shell(void);
char		*get_hostname(void);
void		get_shlvl(t_msh *sh);

/* ************************************************************************** */
/*                                  SIGNALS                                   */
/* ************************************************************************** */

void		set_interactive_signals(void);
void		surpress_par_sig(struct sigaction *o_int, struct sigaction *o_quit);
void		restore_par_sig(struct sigaction *o_int, struct sigaction *o_quit);
void		setup_heredoc_signals(void);
void		reset_child_signals(void);

/* ************************************************************************** */
/*                                   PARSER                                   */
/* ************************************************************************** */

t_syntax	check_cmd_syntax(t_token *tokens, bool print);
t_token		*parse_input(const char *line, t_msh *sh);
char		*expand_one(const char *str, int *i, t_msh *sh);
char		*expand_token(const char *val, t_msh *sh, t_token *token,
				bool *expanded);
void		handle_single_quote(const char *str, int *i, char **res);
void		handle_double_quote(const char *str, int *i, t_msh *sh, char **res);
void		handle_dollar(const char *str, int *i, t_msh *sh, char **res);

void		handle_dollar_quote(const char *str, int *i, char **res);
int			handle_all(const char *str, int *i, t_msh *sh, char **res);

/* ************************************************************************** */
/*                                 TOKENIZER                                  */
/* ************************************************************************** */

int			process_token(t_token **tokens, t_cmd **cur, t_msh *sh,
				bool *cmd_started);
int			is_special_char(char c);
char		*extract_special_char(const char *str, int *i);
char		*unescape_token(const char *str);
t_token		*token_new(char *value, t_token_type type, t_msh *sh);
void		token_add_back(t_token **list, t_token *new_token);
char		**split_input(const char *str);
char		*get_next_segment(const char *str, int *i);

/* ************************************************************************** */
/*                                 EXECUTION                                  */
/* ************************************************************************** */

int			exec_ast(t_token *tokens, t_msh *sh);
int			exec_pipeline(t_cmd *cmds, t_msh *sh, int in_fd);
int			run_single_cmd(t_cmd **cmds, t_msh *sh, int *in_fd, pid_t *pid_out);
int			run_builtin_in_parent(t_cmd *cmd, t_msh *sh);
int			execute_all(t_cmd *cmds, t_msh *sh, int *in_fd, pid_t *pids);
int			prepare_pipe(int pipe_fd[2], int has_next);
int			fork_command(t_cmd *cmd, t_msh *sh, int *in_fd, int pipe_fd[2]);
void		handle_parent_cleanup(int *in_fd, int pipe_fd[2], int has_next);
int			wait_all_children(pid_t *pids, int count);

/* ************************************************************************** */
/*                               CHILD PROCESS                                */
/* ************************************************************************** */

void		exec_child(t_cmd *cmds, int in_fd, int pipe_fd[2], t_msh *sh);

/* ************************************************************************** */
/*                                  BUILTINS                                  */
/* ************************************************************************** */

int			run_builtin(t_cmd *cmd, t_msh *sh);
t_cmd		*build_cmd_list(t_token *tokens, t_msh *sh);
void		finalize_cmds(t_cmd *cmd);
int			cmd_cd(t_cmd *cmd, t_msh *sh);
int			cmd_echo(t_cmd *cmd);
int			cmd_env(t_cmd *cmd, t_msh *sh);
int			cmd_exit(t_cmd *cmd, t_msh *sh);
int			cmd_pwd(t_cmd *cmd);
int			cmd_unset(t_cmd *cmd, t_msh *sh);

// EXPORT
void		export_invalid_identifier(const char *arg);
void		export_assign(const char *arg, t_msh *sh);
int			cmd_export(t_cmd *cmd, t_msh *sh);

/* ************************************************************************** */
/*                            COMMAND CONSTRUCTION                            */
/* ************************************************************************** */

t_cmd		*cmd_new(void);
t_cmd		*cmd_from_tokens(t_token *tokens, t_msh *sh);
char		*get_cmd_path(char *cmd, char **env);
int			add_arg(t_cmd *cmd, char *value);
t_cmd		*reverse_cmd_list(t_cmd *cmd);

/* ************************************************************************** */
/*                          REDIRECTIONS & HEREDOCS                           */
/* ************************************************************************** */

// CORE
int			process_redirect(t_cmd *cmd, t_token *token, t_msh *sh);
int			setup_redirections(t_cmd *cmds, t_msh *sh, int in_fd,
				int pipe_fd[2]);

// REDIRECT PARSING
void		add_redirect(t_cmd *cmd, t_redirect_type type,
				const char *filename);
int			handle_redirect_in(t_cmd *cmd, t_token *file_tok);
int			handle_redirect_out(t_cmd *cmd, t_token *file_tok, bool append);
int			handle_redirect_heredoc(t_cmd *cmd, t_msh *sh, t_token *file_tok);

// REDIRECT EXECUTION
int			apply_all_redirects(t_cmd *cmd, t_msh *sh);
int			apply_single_redirect(t_cmd *cmd, t_redirect *redir, t_msh *sh);
int			open_fd_for_redirect(t_redirect *redir);
int			redirect_and_close(int fd, int target_fd, t_msh *sh);
void		handle_dup2_failure(t_msh *sh);

// FILE PERMISSION CHECKS
int			check_input_file(t_cmd *cmd, const char *filename);
int			check_output_permission(t_cmd *cmd, t_redirect *redir);

// HEREDOC UTIL
char		*write_heredoc_to_tmp(const char *delim, int index, t_msh *sh, t_cmd *cmd);
char		*handle_expansion(char *line, t_msh *sh);

/* ************************************************************************** */
/*                                ENVIRONMENT                                 */
/* ************************************************************************** */

char		*extract_var(char *var, char **env);
int			set_var(char *var, char *val, char ***env);
char		**env_add_var(char **env, char *new_var);
void		sort_env(char **env);
char		**copy_env_array(char **env);

/* ************************************************************************** */
/*                                   ERRORS                                   */
/* ************************************************************************** */

int			exit_error(char *msg, int status);
void		print_heredoc_warning(const char *delim);
void		print_redirect_error(t_cmd *cmds);
void		print_syntax_error(const char *token, bool print);
void		print_syntax_error_eof(bool print);

/* ************************************************************************** */
/*                                  CLEANUP                                   */
/* ************************************************************************** */

void		free_split(char **split);
void		clean_fds(void);
void		free_one_cmd(t_cmd *cmd);
void		free_cmd(t_cmd *cmd);
void		free_token_list(t_token *token);
void		free_env_array(char **env);
void		free_minishell(t_msh *sh);
void		free_hc_minishell(t_msh *sh);
void		free_hc_tokens(t_token *token);
void		free_heredoc_tmpfiles(t_hdoc_tmpfile *tmp);
void		free_final_minishell(t_msh *sh);

/* ************************************************************************** */
/*                                   UTILS                                    */
/* ************************************************************************** */

char		*build_prompt(t_msh *sh);
bool		needs_pipe_continuation(const char *line);

#endif
