/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:07:31 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/03 21:18:55 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*token_new(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	return (token);
}

void	token_add_back(t_token **list, t_token *new_token)
{
	t_token	*temp;

	if (!list || !new_token)
		return ;
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

int	process_token(t_token **tokens, t_cmd **current)
{
	if ((*tokens)->type == TOKEN_WORD)
		add_arg(*current, (*tokens)->value);
	else if ((*tokens)->type == TOKEN_PIPE)
	{
		(*current)->next = cmd_new();
		if (!(*current)->next)
			return (-1);
		*current = (*current)->next;
	}
	else if ((*tokens)->type == TOKEN_REDIRECT_IN
		|| (*tokens)->type == TOKEN_REDIRECT_OUT
		|| (*tokens)->type == TOKEN_APPEND)
	{
		if (process_redirect(*current, *tokens) == -1)
			return (-1);
		*tokens = (*tokens)->next;
	}
	return (0);
}
