/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:39:09 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/15 03:01:41 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_builtin_cmd(const char *cmd)
{
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (true);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (true);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (true);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (true);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (true);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (true);
	return (false);
}

static t_cmd	*build_cmd_list(t_token *tokens)
{
	t_cmd	*first;
	t_cmd	*current;

	first = NULL;
	current = NULL;
	while (tokens)
	{
		if (!current)
		{
			current = cmd_new();
			if (!current)
				return (NULL);
			if (!first)
				first = current;
		}
		if (process_token(&tokens, &current) == -1)
			return (free_cmd(first), NULL);
		if (current->input_fd == -1 || current->output_fd == -1)
			while (tokens && tokens->type != TOKEN_PIPE)
				tokens = tokens->next;
		else
			tokens = tokens->next;
	}
	return (first);
}

static void	finalize_cmds(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->argv && cmd->argv[0])
			cmd->is_builtin = is_builtin_cmd(cmd->argv[0]);
		cmd = cmd->next;
	}
}

t_cmd	*cmd_from_tokens(t_token *tokens)
{
	t_cmd	*cmd;

	cmd = build_cmd_list(tokens);
	if (!cmd)
		return (NULL);
	finalize_cmds(cmd);
	return (cmd);
}

int	run_builtin(t_cmd *cmd, t_minishell *sh)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (FAILURE);
	if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
		return (cmd_env(cmd, sh));
	else if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		return (cmd_echo(cmd));
	else if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		return (cmd_export(cmd, sh));
	else if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
		return (cmd_cd(cmd, sh));
	else if (ft_strncmp(cmd->argv[0], "exit", 4) == 0)
		return (cmd_exit(cmd, sh));
	return (FAILURE);
}
