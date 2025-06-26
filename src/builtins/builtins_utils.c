/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:39:09 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 17:04:56 by paalexan         ###   ########.fr       */
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

static t_cmd	*init_command_skip_empty(t_token **tokens)
{
	t_cmd	*cmd;

	cmd = cmd_new();
	if (!cmd)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_WORD
		&& (*tokens)->expanded_empty)
		*tokens = (*tokens)->next;
	return (cmd);
}

static int	tokens_loop(t_token **tok, t_cmd *cmd, t_msh *sh, bool *cmd_started)
{
	while (*tok && (*tok)->type != TOKEN_PIPE)
	{
		if ((*tok)->type == TOKEN_WORD
			&& (*tok)->expanded_empty && (*tok)->quoted == false)
		{
			*tok = (*tok)->next;
			continue ;
		}
		if (process_token(tok, &cmd, sh, cmd_started) == FAILURE)
			return (FAILURE);
		if (cmd_started && cmd->argv && cmd->argv[0] && cmd->argv[0][0] == '\0')
		{
			cmd->is_valid = false;
			while (*tok && (*tok)->type != TOKEN_PIPE)
				*tok = (*tok)->next;
			break ;
		}
	}
	return (SUCCESS);
}

static t_cmd	*process_command_node(t_token **tokens, t_msh *sh)
{
	t_cmd	*cmd;
	bool	cmd_started;

	cmd_started = false;
	cmd = init_command_skip_empty(tokens);
	if (!cmd)
		return (NULL);
	if (tokens_loop(tokens, cmd, sh, &cmd_started) == FAILURE)
	{
		free_cmd(cmd);
		return (NULL);
	}
	skip_pipe_token(tokens);
	if (!cmd->argv && !cmd->is_valid)
	{
		free_cmd(cmd);
		return (NULL);
	}
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
		{
			free_cmd(first);
			return (NULL);
		}
		if (!first)
			first = new_cmd;
		else
		{
			last->next = new_cmd;
			new_cmd->prev = last;
		}
		last = new_cmd;
	}
	return (first);
}
