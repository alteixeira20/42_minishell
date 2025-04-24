/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:38:08 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/24 16:36:41 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	redirect_fd(int in_fd, int target_fd, t_msh *sh)
{
	if (dup2(in_fd, target_fd) == -1)
	{
		if (!sh->error_printed)
			sh->error_printed = true;
		return (FAILURE);
	}
	close(in_fd);
	return (SUCCESS);
}

static int	handle_stdin_fd(int in_fd, t_cmd *cmd, t_msh *sh)
{
	if (cmd->input_fd == STDIN_FILENO && in_fd != STDIN_FILENO)
		return (redirect_fd(in_fd, STDIN_FILENO, sh));
	else if (in_fd != STDIN_FILENO)
		close(in_fd);
	return (SUCCESS);
}

static int	handle_pipe_out(t_cmd *cmd, int pipe_fd[2], t_msh *sh)
{
	if (cmd->next && pipe_fd[1] != -1)
		return (redirect_fd(pipe_fd[1], STDOUT_FILENO, sh));
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	return (SUCCESS);
}

static void	close_pipe_input(int pipe_fd[2])
{
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
}

int	setup_redirections(t_cmd *cmd, t_msh *sh, int in_fd, int pipe_fd[2])
{
	if (handle_stdin_fd(in_fd, cmd, sh) == FAILURE)
	{
		cmd->redirect_failed = true;
		cmd->redirect_failed_path = ft_strdup("stdin");
		return (FAILURE);
	}
	if (handle_pipe_out(cmd, pipe_fd, sh) == FAILURE)
	{
		cmd->redirect_failed = true;
		cmd->redirect_failed_path = ft_strdup("pipe");
		return (FAILURE);
	}
	close_pipe_input(pipe_fd);
	if (apply_all_redirects(cmd, sh) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
