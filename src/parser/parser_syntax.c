/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 00:46:26 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 13:43:51 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_redirect(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN
		|| type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND
		|| type == TOKEN_HEREDOC);
}

static t_syntax	check_pipe_syntax(t_token *token, bool print)
{
	t_token	*cur;

	cur = token;
	while (cur)
	{
		if (cur->type == TOKEN_PIPE)
		{
			if (!cur->next)
			{
				print_syntax_error("|", print);
				return (SYNTAX_INCOMPLETE);
			}
			if (cur->next->type == TOKEN_PIPE)
			{
				print_syntax_error("|", print);
				return (SYNTAX_ERROR);
			}
		}
		cur = cur->next;
	}
	return (SYNTAX_OK);
}

static t_syntax	check_redirect_syntax(t_token *token, bool print)
{
	if (!token->next || token->next->type != TOKEN_WORD)
	{
		if (print)
			ft_putendl_fd("syntax error near unexpected token `newline'", 2);
		return (SYNTAX_ERROR);
	}
	return (SYNTAX_OK);
}

static t_syntax	check_token_sequence(t_token *tokens, bool print)
{
	t_token		*current;
	t_syntax	result;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			result = check_pipe_syntax(current, print);
			if (result != SYNTAX_OK)
				return (result);
		}
		else if (is_redirect(current->type))
		{
			result = check_redirect_syntax(current, print);
			if (result != SYNTAX_OK)
				return (result);
		}
		current = current->next;
	}
	return (SYNTAX_OK);
}

t_syntax	check_cmd_syntax(t_token *tokens, bool print)
{
	if (!tokens)
		return (SYNTAX_OK);
	if (tokens->type == TOKEN_PIPE)
	{
		print_syntax_error("|", print);
		return (SYNTAX_ERROR);
	}
	return (check_token_sequence(tokens, print));
}
