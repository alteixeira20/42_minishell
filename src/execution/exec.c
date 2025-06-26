/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:31:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 13:28:05 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exec_handle_invalid_cmd(t_cmd **cmds, int *in_fd)
{
	if (*in_fd != STDIN_FILENO)
	{
		close(*in_fd);
		*in_fd = STDIN_FILENO;
	}
	if ((*cmds)->argv && (*cmds)->argv[0])
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((*cmds)->argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else
		ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
	g_exit = 127;
	*cmds = (*cmds)->next;
}

static int	exec_handle_failed_redirect(t_cmd **cmds, pid_t *pids, int *i)
{
	if (*cmds && (*cmds)->redirect_failed)
	{
		pids[*i] = -1;
		(*i)++;
		return (1);
	}
	return (0);
}

static void	exec_reset_in_fd_if_needed(t_cmd *cmds, int *in_fd)
{
	if (!cmds)
		*in_fd = STDIN_FILENO;
}

int	execute_all(t_cmd *cmds, t_msh *sh, int *in_fd, pid_t *pids)
{
	int		i;

	i = 0;
	while (cmds)
	{
		if (!cmds->argv || !cmds->argv[0] || !cmds->is_valid)
		{
			exec_handle_invalid_cmd(&cmds, in_fd);
			continue ;
		}
		if (run_single_cmd(&cmds, sh, in_fd, &pids[i]) == FAILURE)
		{
			if (exec_handle_failed_redirect(&cmds, pids, &i))
				continue ;
			return (FAILURE);
		}
		exec_reset_in_fd_if_needed(cmds, in_fd);
		i++;
	}
	if (*in_fd != STDIN_FILENO)
	{
		close(*in_fd);
		*in_fd = STDIN_FILENO;
	}
	return (SUCCESS);
}

int	exec_ast(t_token *tokens, t_msh *sh)
{
	t_cmd	*cmds;
	int		in_fd;
	int		status;


	status = 0;
	sh->error_printed = false;
	cmds = cmd_from_tokens(tokens, sh);
	if (!cmds)
	{
		g_exit = 0;
		return (SUCCESS);
	}
	in_fd = 0;
	status = exec_pipeline(cmds, sh, in_fd);
	free_cmd(cmds);
	g_exit = status;
	return (status);
}
