/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 22:39:18 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 16:42:28 by paalexan         ###   ########.fr       */
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

static int	exec_single_builtin(t_cmd *cmds, t_msh *sh)
{
	int	status;

	status = run_builtin_in_parent(cmds, sh);
	return (status);
}

static int	exec_prepare_pids(t_cmd *cmds, t_msh *sh, int *cmd_count)
{
	*cmd_count = count_commands(cmds);
	sh->pids = ft_calloc(*cmd_count, sizeof(pid_t));
	if (!sh->pids)
	{
		free(sh->pids);
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	exec_pipeline_sequence(t_cmd *cmds, t_msh *sh,
								int *in_fd, int *cmd_count)
{
	if (has_valid_cmd(cmds) == FAILURE)
	{
		print_redirect_error(cmds);
		return (127);
	}
	if (exec_prepare_pids(cmds, sh, cmd_count) == FAILURE)
	{
		free(sh->pids);
		return (FAILURE);
	}
	if (execute_all(cmds, sh, in_fd, sh->pids) == FAILURE)
	{
		free(sh->pids);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	exec_pipeline(t_cmd *cmds, t_msh *sh, int in_fd)
{
	int		cmd_count;
	int		status;
	t_cmd	*cmds_head;

	sh->pids = NULL;
	sh->is_heredoc = false;
	if (!cmds)
		return (FAILURE);
	cmds_head = cmds;
	if (!cmds->next && cmds->is_builtin)
		return (exec_single_builtin(cmds, sh));
	if (exec_pipeline_sequence(cmds_head, sh, &in_fd, &cmd_count) != SUCCESS)
		return (127);
	status = wait_all_children(sh->pids, cmd_count);
	print_redirect_error(cmds_head);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	return (status);
}
