/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:39:09 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/18 01:35:32 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	skip_pipe_token(t_token **tokens)
{
	if (!tokens || !*tokens)
		return ;
	if ((*tokens)->next)
		*tokens = (*tokens)->next;
	else
		*tokens = NULL;
}

static int	handle_fd_failure(t_cmd *cmd, t_token **tokens)
{
	free_cmd(cmd);
	skip_pipe_token(tokens);
	return (1);
}

static t_cmd	*process_command_node(t_token **tokens, t_msh *sh)
{
	t_cmd	*cmd;

	cmd = cmd_new();
	if (!cmd)
		return (NULL);
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if (process_token(tokens, &cmd, sh) == -1)
			return (free_cmd(cmd), NULL);
	}
	if (cmd->input_fd == -1 || cmd->output_fd == -1)
	{
		if (handle_fd_failure(cmd, tokens))
			return (NULL);
	}
	skip_pipe_token(tokens);
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
		new_cmd = process_command_node(&tokens, sh);
		if (!new_cmd)
			continue ;
		if (!first)
			first = new_cmd;
		else
			last->next = new_cmd;
		last = new_cmd;
	}
	return (first);
}
