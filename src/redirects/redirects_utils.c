/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 20:43:21 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/10 21:48:47 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_redirections(t_cmd *cmds, int in_fd, int pipe_fd[2])
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cmds->input_fd != STDIN_FILENO)
	{
		dup2(cmds->input_fd, STDIN_FILENO);
		close(cmds->input_fd);
	}
	if (cmds->output_fd != STDOUT_FILENO)
		dup2(cmds->output_fd, STDOUT_FILENO);
	else if (cmds->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}
