/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 21:22:46 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/24 17:08:38 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	prepare_pipe(int pipe_fd[2], int has_next)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (has_next && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (FAILURE);
	}
	return (SUCCESS);
}

int	fork_command(t_cmd *cmd, t_msh *sh, int *in_fd, int pipe_fd[2])
{
	pid_t				pid;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	surpress_par_sig(&old_int, &old_quit);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), FAILURE);
	if (pid == 0)
		exec_child(cmd, *in_fd, pipe_fd, sh);
	restore_par_sig(&old_int, &old_quit);
	return (pid);
}

void	handle_parent_cleanup(int *in_fd, int pipe_fd[2], int has_next)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	if (has_next)
		*in_fd = pipe_fd[0];
	else if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
}

int	run_single_cmd(t_cmd **cmds, t_msh *sh, int *in_fd, pid_t *pid_out)
{
	int		pipe_fd[2];
	int		has_next;

	has_next = (*cmds)->next != NULL;
	if (prepare_pipe(pipe_fd, has_next) == FAILURE)
	{
		*pid_out = -1;
		handle_parent_cleanup(in_fd, pipe_fd, has_next);
		*cmds = (*cmds)->next;
		return (FAILURE);
	}
	*pid_out = fork_command(*cmds, sh, in_fd, pipe_fd);
	if (*pid_out == FAILURE)
	{
		*pid_out = -1;
		handle_parent_cleanup(in_fd, pipe_fd, has_next);
		*cmds = (*cmds)->next;
		return (FAILURE);
	}
	handle_parent_cleanup(in_fd, pipe_fd, has_next);
	*cmds = (*cmds)->next;
	return (SUCCESS);
}

int	wait_all_children(pid_t *pids, int count)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	g_exit = last_status;
	return (last_status);
}
