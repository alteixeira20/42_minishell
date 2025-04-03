/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:39:09 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/03 21:33:22 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_builtin_cmd(const char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
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
			return (NULL);
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
