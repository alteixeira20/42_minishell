/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopedro- <jopedro-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:10:29 by jopedro-          #+#    #+#             */
/*   Updated: 2025/06/25 14:17:00 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_hc_tokens(t_token *token)
{
	t_token	*next;

	if (!token)
		return ;
	while (token)
	{
		next = token->next;
		free(token->value);
		free(token);
		token = next;
	}
}

void	free_heredoc_tmpfiles(t_hdoc_tmpfile *lst)
{
	t_hdoc_tmpfile	*tmp;

	while (lst && lst->prev)
		lst = lst->prev;
	while (lst)
	{
		tmp = lst->next;
		free(lst->path);
		free(lst);
		lst = tmp;
	}
}

void	free_hc_minishell(t_msh *sh)
{
	if (sh->home)
		free(sh->home);
	if (sh->user)
		free(sh->user);
	if (sh->hostname)
		free(sh->hostname);
	if (sh->tokens)
		free_token_list(sh->tokens);
	if (sh->heredoc_tmpfiles)
		free_heredoc_tmpfiles(sh->heredoc_tmpfiles);
}

