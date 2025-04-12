/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:31:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/12 04:12:38 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	close_parent_pipe(int *in_fd, int pipe_fd[2], t_cmd *cmds)
{
	if (*in_fd != 0)
		close(*in_fd);
	if (cmds->next)
	{
		close(pipe_fd[1]);
		*in_fd = pipe_fd[0];
	}
	else
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

static int	run_single_command(t_cmd **cmds, t_minishell *sh, int *in_fd)
{
	pid_t				pid;
	int					pipe_fd[2];
	int					status;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	if ((*cmds)->next && pipe(pipe_fd) == -1)
		return (perror("pipe"), FAILURE);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), FAILURE);
	if (pid == 0)
		exec_child(*cmds, *in_fd, pipe_fd, sh);
	surpress_parent_sig(&old_int, &old_quit);
	waitpid(pid, &status, 0);
	restore_parent_sig(&old_int, &old_quit);
	if ((*cmds)->next)
		close_parent_pipe(in_fd, pipe_fd, *cmds);
	else if (*in_fd != 0)
		close(*in_fd);
	*cmds = (*cmds)->next;
	return (SUCCESS);
}

int	exec_pipeline(t_cmd *cmds, t_minishell *sh, int in_fd)
{
	if (!cmds->next && cmds->is_builtin)
		return (run_builtin(cmds, sh));
	while (cmds)
	{
		if (run_single_command(&cmds, sh, &in_fd) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	exec_tokens(t_token *tokens, t_minishell *sh)
{
	t_cmd	*cmds;
	int		in_fd;

	cmds = cmd_from_tokens(tokens);
	if (!cmds)
		return (FAILURE);
	in_fd = 0;
	exec_pipeline(cmds, sh, in_fd);
	free_cmd(cmds);
	return (SUCCESS);
}
