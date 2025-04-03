/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:31:36 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/03 22:26:23 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		free(cmd->argv[i]);
		i++;
	}
	free(cmd->argv);
	free(cmd);
}

void	free_token_list(t_token *token)
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
