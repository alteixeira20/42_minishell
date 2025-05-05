/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:19:32 by paalexan          #+#    #+#             */
/*   Updated: 2025/05/06 00:45:43 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_heredoc_warning(const char *delim)
{
	ft_putstr_fd("warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd((char *)delim, STDERR_FILENO);
	ft_putstr_fd("'=\n", STDERR_FILENO);
}

void	print_redirect_error(t_cmd *cmds)
{
	t_cmd	*tmp;

	tmp = cmds;
	while (tmp)
	{
		if (tmp->redirect_failed && tmp->redirect_failed_path)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(tmp->redirect_failed_path);
			free(tmp->redirect_failed_path);
			tmp->redirect_failed_path = NULL;
			break ;
		}
		tmp = tmp->next;
	}
}

int	exit_error(char *msg, int status)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	g_exit = status;
	return (FAILURE);
}
