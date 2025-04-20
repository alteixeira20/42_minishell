/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:31:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/20 00:53:03 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_commands(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	execute_all(t_cmd *cmds, t_msh *sh, int *in_fd, pid_t *pids)
{
	int	i;
	int	has_error;

	i = 0;
	has_error = 0;
	while (cmds)
	{
		if (run_single_cmd(&cmds, sh, in_fd, &pids[i]) == FAILURE)
		{
			pids[i] = -1;
			has_error = 1;
		}
		i++;
	}
	if (has_error)
		return (FAILURE);
	return (SUCCESS);
}

static int	run_single_builtin_in_parent(t_cmd *cmd, t_msh *sh)
{
	int	saved_stdin;
	int	saved_stdout;
	int	dummy_pipe[2];

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	dummy_pipe[0] = -1;
	dummy_pipe[1] = -1;
	setup_redirections(cmd, sh, STDIN_FILENO, dummy_pipe);
	g_exit = run_builtin(cmd, sh);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (g_exit);
}

int	exec_pipeline(t_cmd *cmds, t_msh *sh, int in_fd)
{
	int		cmd_count;
	pid_t	*pids;
	int		status;

	if (!cmds)
		return (FAILURE);
	if (!cmds->next && cmds->is_builtin)
		return (run_single_builtin_in_parent(cmds, sh));
	cmd_count = count_commands(cmds);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (FAILURE);
	execute_all(cmds, sh, &in_fd, pids);
	status = wait_all_children(pids, cmd_count);
	free(pids);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	return (status);
}

int	exec_ast(t_token *tokens, t_msh *sh)
{
	t_cmd	*cmds;
	int		in_fd;
	int		status;

	sh->error_printed = false;
	cmds = cmd_from_tokens(tokens, sh);
	if (!cmds)
		return (FAILURE);
	in_fd = 0;
	status = exec_pipeline(cmds, sh, in_fd);
	g_exit = status;
	free_cmd(cmds);
	return (status);
}
