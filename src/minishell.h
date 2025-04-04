/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:02:36 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/04 03:25:11 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Standard Headers
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>

// Custom Headers
# include "../lib/libft/libft/libft.h"
# include "../lib/libft/gnl/get_next_line_bonus.h"

// Readline Headers
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

extern int	g_exit;

//Macros
# define NO_ENV -1
# define NO_VAR -1
# define INIT_ERR		"Init Error\n"
# define MALLOC_ERR		"Malloc Error\n"
# define ENV_INIT_ERR	"Env Init Error\n"

//Init Prototypes
int		init(t_minishell *sh, char **env);
char	**init_env(char **env);

// Commands Prototypes
int		cmd_echo(t_cmd *cmd);
t_cmd	*cmd_from_tokens(t_token *tokens);
char	*get_cmd_path(char *cmd, char **env);
t_cmd	*cmd_new(void);
int		add_arg(t_cmd *cmd, char *value);
int		run_builtin(t_cmd *cmd);
char	*build_cwd(char *cwd, char *home);
char	*build_prompt(t_minishell *sh);

// Tokens Prototypes
t_token	*token_new(char *value, t_token_type type);
void	token_add_back(t_token **list, t_token *new_token);
int		exec_tokens(t_token *tokens, t_minishell *sh);
int		process_token(t_token **tokens, t_cmd **current);

// Redirects Prototypes
int		process_redirect(t_cmd *cmd, t_token *token);

// Parser Prototypes
t_token	*parse_input(const char *line);
char	**split_input(const char *str);

//env Prototypes
char	*extract_var(char *var, char **env);
int		set_var(char *var, char *val, char ***env);
char	**env_add_var(char **env, char *new_var);

//Error Prototypes
int		exit_error(char *msg, int status);

// Free Prototypes
void	free_split(char **split);
void	free_cmd(t_cmd *cmd);
void	free_token_list(t_token *token);

#endif
