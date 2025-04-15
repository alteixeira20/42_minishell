/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:31:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/15 00:25:20 by paalexan         ###   ########.fr       */
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

int	execute_all(t_cmd *cmds, t_minishell *sh, int *in_fd, pid_t *pids)
{
	int	i;

	i = 0;
	while (cmds)
	{
		if (run_single_command(&cmds, sh, in_fd, &pids[i]) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int	exec_pipeline(t_cmd *cmds, t_minishell *sh, int in_fd)
{
	int		cmd_count;
	pid_t	*pids;

	if (!cmds->next && cmds->is_builtin
		&& in_fd == STDIN_FILENO
		&& cmds->input_fd == STDIN_FILENO
		&& cmds->output_fd == STDOUT_FILENO)
		return (run_builtin(cmds, sh));
	cmd_count = count_commands(cmds);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (FAILURE);
	if (execute_all(cmds, sh, &in_fd, pids) == FAILURE)
		return (free(pids), FAILURE);
	wait_all_children(pids, cmd_count);
	free(pids);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
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
