/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:39:09 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/17 16:46:20 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_builtin_cmd(const char *cmd)
{
	if (ft_strcmp(cmd, "env") == 0)
		return (true);
	if (ft_strcmp(cmd, "echo") == 0)
		return (true);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "export") == 0)
		return (true);
	if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	return (false);
}

static t_cmd	*process_command_node(t_token *tokens, t_msh *sh)
{
	t_cmd	*cmd;

	cmd = cmd_new();
	if (!cmd)
		return (NULL);
	if (process_token(tokens, &cmd, sh) == -1)
	{
		free_cmd(cmd);
		return (NULL);
	}
	if (cmd->input_fd == -1 || cmd->output_fd == -1)
	{
		while (tokens && tokens->type != TOKEN_PIPE)
			tokens = tokens->next;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
		free_cmd(cmd);
		return (NULL);
	}
	if (tokens && tokens->type == TOKEN_PIPE)
		tokens = tokens->next;
	return (cmd);
}

t_cmd	*build_cmd_list(t_token *tokens, t_msh *sh)
{
	t_cmd	*first;
	t_cmd	*last;
	t_cmd	*new_cmd;

	first = NULL;
	last = NULL;
	while (tokens)
	{
		new_cmd = process_command_node(tokens, sh);
		while (tokens && tokens->type != TOKEN_PIPE)
			tokens = tokens->next;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
		if (!new_cmd)
			continue;
		if (!first)
			first = new_cmd;
		if (last)
			last->next = new_cmd;
		last = new_cmd;
	}
	return (first);
}

void	finalize_cmds(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->argv && cmd->argv[0])
			cmd->is_builtin = is_builtin_cmd(cmd->argv[0]);
		cmd = cmd->next;
	}
}

int	run_builtin(t_cmd *cmd, t_msh *sh)
{
	if (!cmd || !cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
		return (FAILURE);
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (cmd_env(cmd, sh));
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (cmd_echo(cmd));
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (cmd_export(cmd, sh));
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (cmd_cd(cmd, sh));
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (cmd_exit(cmd, sh));
	return (FAILURE);
}
