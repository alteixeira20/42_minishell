/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_parent.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:09:21 by paalexan          #+#    #+#             */
/*   Updated: 2025/05/07 00:21:52 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	save_std_fds(int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
}

static void	restore_std_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static int	handle_empty_builtin(t_cmd *cmd, t_msh *sh)
{
	int	stdin_backup;
	int	stdout_backup;

	save_std_fds(&stdin_backup, &stdout_backup);
	if (setup_redirections(cmd, sh, STDIN_FILENO, NULL) == FAILURE)
	{
		restore_std_fds(stdin_backup, stdout_backup);
		return (g_exit);
	}
	restore_std_fds(stdin_backup, stdout_backup);
	return (SUCCESS);
}

static int	handle_invalid_builtin(t_cmd *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd->argv[0])
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	g_exit = 127;
	return (g_exit);
}

int	run_builtin_in_parent(t_cmd *cmd, t_msh *sh)
{
	int	stdin_backup;
	int	stdout_backup;
	int	dummy_pipe[2];

	if (!cmd->argv || !cmd->argv[0])
		return (handle_empty_builtin(cmd, sh));
	if (cmd->argv[0][0] == '\0' || !cmd->is_valid)
		return (handle_invalid_builtin(cmd));
	save_std_fds(&stdin_backup, &stdout_backup);
	dummy_pipe[0] = -1;
	dummy_pipe[1] = -1;
	if (setup_redirections(cmd, sh, STDIN_FILENO, dummy_pipe) == FAILURE)
	{
		print_redirect_error(cmd);
		restore_std_fds(stdin_backup, stdout_backup);
		return (g_exit);
	}
	g_exit = run_builtin(cmd, sh);
	restore_std_fds(stdin_backup, stdout_backup);
	return (g_exit);
}
