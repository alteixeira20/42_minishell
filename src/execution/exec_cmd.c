/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 03:55:09 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/16 16:47:59 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	add_arg(t_cmd *cmd, char *value)
{
	int		i;
	char	**new_argv;

	if (!value)
		return (FAILURE);
	i = 0;
	while (cmd->argv && cmd->argv[i])
		i++;
	new_argv = malloc(sizeof(char *) * (i + 2));
	if (!new_argv)
		return (FAILURE);
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i++] = ft_strdup(value);
	new_argv[i] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
	cmd->argc++;
	return (SUCCESS);
}

t_cmd	*reverse_cmd_list(t_cmd *cmd)
{
	t_cmd	*prev;
	t_cmd	*next;

	prev = NULL;
	while (cmd)
	{
		next = cmd->next;
		cmd->next = prev;
		prev = cmd;
		cmd = next;
	}
	return (prev);
}

t_cmd	*cmd_from_tokens(t_token *tokens, t_msh *sh)
{
	t_cmd	*cmd;

	cmd = build_cmd_list(tokens, sh);
	if (!cmd)
		return (NULL);
	finalize_cmds(cmd);
	sh->cmds = cmd;
	return (cmd);
}

bool	has_redirection_error(t_cmd *cmds)
{
	while (cmds)
	{
		if (cmds->input_fd == -1 || cmds->output_fd == -1)
			return (true);
		cmds = cmds->next;
	}
	return (false);
}
