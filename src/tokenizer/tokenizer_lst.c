/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 00:44:36 by paalexan          #+#    #+#             */
/*   Updated: 2025/05/05 18:22:26 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	helper_tok(t_token *token, char *val, t_token_type type, t_msh *sh)
{
	bool	expanded;
	char	*tmp;

	if (type == TOKEN_WORD)
	{
		token->quoted = false;
		expanded = false;
		token->value = expand_token(val, sh, token, &expanded);
		if (!token->value)
			return ;
		if (token->value[0] == '\0' && expanded)
			token->expanded_empty = true;
		else
			token->expanded_empty = false;
	}
	else
	{
		if (type == TOKEN_HEREDOC)
		{
			tmp = expand_token(val, sh, token, &expanded);
			free(tmp);
		}
		token->value = ft_strdup(val);
		token->expanded_empty = false;
	}
}

t_token	*token_new(char *value, t_token_type type, t_msh *sh)
{
	t_token	*token;

	token = init_token();
	if (!token)
		return (NULL);
	token->type = type;
	helper_tok(token, value, type, sh);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	return (token);
}

void	token_add_back(t_token **list, t_token *new_token)
{
	t_token	*temp;

	if (!list || !new_token)
		return ;
	if (new_token->type != TOKEN_WORD && ft_strlen(new_token->value) == 0)
	{
		free(new_token->value);
		free(new_token);
		return ;
	}
	if (*list == NULL)
	{
		*list = new_token;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
}
