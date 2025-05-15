/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:31:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/05/15 19:25:08 by paalexan         ###   ########.fr       */
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
	int		i;

	i = 0;
	while (cmds)
	{
		if (!cmds->argv || !cmds->argv[0] || !cmds->is_valid)
		{
			if (*in_fd != STDIN_FILENO)
			{
				close(*in_fd);
				*in_fd = STDIN_FILENO;
			}
			if (cmds->argv && cmds->argv[0])
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(cmds->argv[0], STDERR_FILENO);
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
			}
			else
				ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
			g_exit = 127;
			cmds = cmds->next;
			continue ;
		}
		if (run_single_cmd(&cmds, sh, in_fd, &pids[i]) == FAILURE)
		{
			if (cmds && cmds->redirect_failed)
			{
				pids[i] = -1;
				i++;
				continue ;
			}
			return (FAILURE);
		}
		if (!cmds)
			*in_fd = STDIN_FILENO;
		i++;
	}
	if (*in_fd != STDIN_FILENO)
	{
		close(*in_fd);
		*in_fd = STDIN_FILENO;
	}
	return (SUCCESS);
}

static int	has_valid_cmd(t_cmd *cmd)
{
	if (!cmd->argv)
		return (FAILURE);
	while (cmd)
	{
		if (cmd->argv && cmd->argv[0] && cmd->is_valid)
			return (SUCCESS);
		if (cmd->argv[0][0] == '\0')
		{
			ft_putstr_fd("Command '' not found\n", STDERR_FILENO);
			return (FAILURE);
		}
		cmd = cmd->next;
	}
	return (FAILURE);
}

int	exec_pipeline(t_cmd *cmds, t_msh *sh, int in_fd)
{
	int		cmd_count;
	pid_t	*pids;
	int		status;
	t_cmd	*cmds_head;

	pids = NULL;
	sh->is_heredoc = false;
	if (!cmds)
		return (FAILURE);
	if (!cmds->next && cmds->is_builtin)
	{
		status = run_builtin_in_parent(cmds, sh);
		if (cmds)
			free_cmd(cmds);
		return (status);
	}
	if (has_valid_cmd(cmds) == FAILURE)
	{
		print_redirect_error(cmds);
		free_cmd(cmds);
		return (127);
	}
	cmds_head = cmds;
	cmd_count = count_commands(cmds);
	pids = ft_calloc(cmd_count, sizeof(pid_t));
	if (!pids)
	{
		free_cmd(cmds);
		return (FAILURE);
	}
	if (execute_all(cmds, sh, &in_fd, pids) == FAILURE)
	{
		free(pids);
		return (FAILURE);
	}
	status = wait_all_children(pids, cmd_count);
	print_redirect_error(cmds_head);
	free(pids);
	free_cmd(cmds_head);
	cmds = NULL;
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
	free_token_list(tokens);
	if (!cmds)
	{
		g_exit = 0;
		return (SUCCESS);
	}
	in_fd = 0;
	status = exec_pipeline(cmds, sh, in_fd);
	g_exit = status;
	return (status);
}
