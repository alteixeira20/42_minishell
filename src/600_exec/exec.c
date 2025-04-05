/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:31:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/05 14:19:23 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exec_child(t_cmd *cmds, int in_fd, int pipe_fd[2], t_minishell *sh)
{
	char	*full_path;

	setup_redirections(cmds, in_fd, pipe_fd);
	if (!cmds->argv || !cmds->argv[0])
	{
		if (cmds->output_fd != STDOUT_FILENO || cmds->input_fd != STDIN_FILENO)
			exit(SUCCESS);
		exit(FAILURE);
	}
	if (cmds->is_builtin)
		exit(run_builtin(cmds, sh));
	full_path = get_cmd_path(cmds->argv[0], sh->env);
	if (full_path)
		execve(full_path, cmds->argv, sh->env);
	if (ft_strchr(cmds->argv[0], '/'))
	{
		perror(cmds->argv[0]);
		exit(127);
	}
	ft_putstr_fd(cmds->argv[0], STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(127);
}

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

int	exec_pipeline(t_cmd *cmds, t_minishell *sh)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;
	int		status;
	bool	has_pipe;

	in_fd = 0;
	while (cmds)
	{
		has_pipe = (cmds->next != NULL);
		if (has_pipe && pipe(pipe_fd) == -1)
			return (perror("pipe"), FAILURE);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), FAILURE);
		if (pid == 0)
			exec_child(cmds, in_fd, pipe_fd, sh);
		if (has_pipe)
			close_parent_pipe(&in_fd, pipe_fd, cmds);
		else if (in_fd != 0)
			close(in_fd);
		waitpid(pid, &status, 0);
		cmds = cmds->next;
	}
	return (SUCCESS);
}

int	exec_tokens(t_token *tokens, t_minishell *sh)
{
	t_cmd	*cmds;

	cmds = cmd_from_tokens(tokens);
	if (!cmds)
		return (FAILURE);
	exec_pipeline(cmds, sh);
	free_cmd(cmds);
	return (SUCCESS);
}
