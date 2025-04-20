/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:07:31 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/19 18:16:53 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*unescape_token(const char *str)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
			i++;
		res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

int	is_special_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

char	*extract_special_char(const char *str, int *i)
{
	int	start;

	start = *i;
	if ((str[*i] == '>' || str[*i] == '<') && str[*i] == str[*i + 1])
		(*i) += 2;
	else
		(*i) += 1;
	return (ft_substr(str, start, *i - start));
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

int	process_token(t_token **tokens, t_cmd **current, t_msh *sh)
{
	t_token	*tok;

	if (!tokens || !*tokens || !current || !*current || !sh)
		return (FAILURE);
	tok = *tokens;
	if (tok->type == TOKEN_WORD)
	{
		if (add_arg(*current, tok->value) == FAILURE)
			return (FAILURE);
		*tokens = tok->next;
	}
	else if (tok->type == TOKEN_PIPE)
		return (SUCCESS);
	else
	{
		if (handle_redirect_token(tok, *current, sh) == FAILURE)
			(*current)->input_fd = -1;
		if (tok->next)
			*tokens = tok->next->next;
		else
			*tokens = NULL;
	}
	return (SUCCESS);
}
