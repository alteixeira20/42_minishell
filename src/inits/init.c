/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:28:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 16:13:36 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	init_sh_util(t_msh *sh, char **env)
{
	char	*pwd;

	sh->env = init_env(env);
	get_shlvl(sh);
	if (sh->env == NULL)
		return (exit_error(ENV_INIT_ERR, errno), FAILURE);
	sh->home = extract_var ("HOME", sh->env);
	sh->user = extract_var("USER", sh->env);
	sh->hostname = get_hostname();
	pwd = extract_var("PWD", sh->env);
	if (pwd)
	{
		set_var("OLDPWD", pwd, &sh->env);
		free(pwd);
	}
	return (SUCCESS);
}

int	init_sh(t_msh *sh, char **env)
{
	if (init_sh_util(sh, env) != SUCCESS)
		return (exit_error(ENV_INIT_ERR, errno), FAILURE);
	sh->cmds = NULL;
	sh->tokens = NULL;
	sh->cmd_cnt = 0;
	sh->pipes = NULL;
	sh->pipe_cnt = 0;
	sh->hd_cnt = 0;
	sh->path = NULL;
	sh->exit_status = 0;
	sh->heredoc_interrupted = false;
	sh->heredoc_tmpfiles = NULL;
	sh->is_heredoc = false;
	sh->error_printed = false;
	return (SUCCESS);
}

t_token	*init_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = NULL;
	token->type = TOKEN_WORD;
	token->expanded_empty = false;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

t_cmd	*cmd_new(void)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argc = 0;
	cmd->argv = NULL;
	cmd->redirects = NULL;
	cmd->redirect_failed = false;
	cmd->redirect_failed_path = NULL;
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->is_builtin = false;
	cmd->is_valid = true;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}
