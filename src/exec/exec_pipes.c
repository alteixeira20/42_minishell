/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 21:22:46 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/15 00:25:27 by paalexan         ###   ########.fr       */
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

int	fork_command(t_cmd *cmd, t_minishell *sh, int *in_fd, int pipe_fd[2])
{
	pid_t				pid;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), FAILURE);
	if (pid == 0)
		exec_child(cmd, *in_fd, pipe_fd, sh);
	surpress_parent_sig(&old_int, &old_quit);
	restore_parent_sig(&old_int, &old_quit);
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

int	run_single_command(t_cmd **cmds, t_minishell *sh,
						int *in_fd, pid_t *pid_out)
{
	int	pipe_fd[2];
	int	has_next;

	has_next = (*cmds)->next != NULL;
	if (prepare_pipe(pipe_fd, has_next) == FAILURE)
		return (FAILURE);
	*pid_out = fork_command(*cmds, sh, in_fd, pipe_fd);
	if (*pid_out == FAILURE)
		return (FAILURE);
	handle_parent_cleanup(in_fd, pipe_fd, has_next);
	*cmds = (*cmds)->next;
	return (SUCCESS);
}

void	wait_all_children(pid_t *pids, int count)
{
	int	i;
	int	status;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
}
