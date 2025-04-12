/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 00:44:36 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/12 04:30:34 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*token_new(char *value, t_token_type type, t_minishell *sh)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (type == TOKEN_WORD)
		token->value = expand_token_value(value, sh);
	else
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
