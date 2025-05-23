/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopedro- <jopedro-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:10:29 by jopedro-          #+#    #+#             */
/*   Updated: 2025/05/23 12:21:55 by jopedro-         ###   ########.fr       */
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

void	free_hc_minishell(t_msh *sh)
{
	if (sh->home)
		free(sh->home);
	if (sh->user)
		free(sh->user);
	if (sh->hostname)
		free(sh->hostname);
}
