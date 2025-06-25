/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:37:47 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/16 16:09:03 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	split_expanded_token(t_token *tok, t_cmd *cmd)
{
	char	**split;
	int		i;

	split = ft_split(tok->value, ' ');
	if (!split)
		return (FAILURE);
	i = 0;
	while (split[i])
	{
		if (add_arg(cmd, split[i]) == FAILURE)
		{
			free_split(split);
			return (FAILURE);
		}
		i++;
	}
	free_split(split);
	return (SUCCESS);
}

static int	handle_word_token(t_token **tokens, t_cmd **cur, bool *cmd_started)
{
	t_token	*tok;

	tok = *tokens;
	if (!(*cmd_started))
	{
		if ((!tok->value || tok->value[0] == '\0')
			&& tok->expanded_empty == false)
			(*cur)->is_valid = false;
		*cmd_started = true;
	}
	if ((*cur)->argc == 0
		&& tok->value
		&& ft_strchr(tok->value, ' ') != NULL
		&& tok->quoted == false)
	{
		if (split_expanded_token(tok, *cur) == FAILURE)
			return (FAILURE);
	}
	else if (tok->value)
	{
		if (add_arg(*cur, tok->value) == FAILURE)
			return (FAILURE);
	}
	*tokens = tok->next;
	return (SUCCESS);
}

static int	handle_redirect_token(t_token *tokens, t_cmd *cmd, t_msh *sh)
{
	if (tokens->type == TOKEN_REDIRECT_IN
		|| tokens->type == TOKEN_REDIRECT_OUT
		|| tokens->type == TOKEN_APPEND)
	{
		if (process_redirect(cmd, tokens, sh) == FAILURE)
			return (FAILURE);
		tokens = tokens->next;
	}
	else if (tokens->type == TOKEN_HEREDOC)
	{
		if (handle_redirect_heredoc(cmd, sh, tokens->next) == FAILURE)
			return (FAILURE);
		tokens = tokens->next;
	}
	return (0);
}

static int	process_redirects(t_token **tokens, t_cmd *cmd, t_msh *sh)
{
	t_token	*tok;

	tok = *tokens;
	if (handle_redirect_token(tok, cmd, sh) == FAILURE)
	{
		cmd->input_fd = -1;
		return (FAILURE);
	}
	if (tok->next)
		*tokens = tok->next->next;
	else
		*tokens = NULL;
	return (SUCCESS);
}

int	process_token(t_token **tokens, t_cmd **cmd, t_msh *sh, bool *cmd_started)
{
	t_token	*tok;

	if (!tokens || !*tokens || !*cmd || !sh)
		return (FAILURE);
	tok = *tokens;
	if (tok->type == TOKEN_WORD)
		return (handle_word_token(tokens, cmd, cmd_started));
	else if (tok->type == TOKEN_PIPE)
		return (SUCCESS);
	else
		return (process_redirects(tokens, *cmd, sh));
}
