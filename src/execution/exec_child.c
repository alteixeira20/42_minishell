/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 03:40:27 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 16:55:50 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exec_builtin_cmd(t_cmd *cmd, t_msh *sh)
{
	int	status;

	reset_child_signals();
	status = run_builtin(cmd, sh);
	free_cmd(cmd);
	free_env_array(sh->env);
	free_minishell(sh);
	free(sh->pids);
	exit(status);
}

static void	exit_if_directory(t_cmd *cmd, t_msh *sh, char *full_path)
{
	struct stat	st;

	if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(full_path, STDERR_FILENO);
		free(full_path);
		free(sh->pids);
		free_cmd(cmd);
		free_env_array(sh->env);
		free_hc_minishell(sh);
		exit(126);
	}
}

static void	try_exec_binary(t_cmd *cmd, t_msh *sh)
{
	char	*full_path;
	int		code;

	full_path = get_cmd_path(cmd->argv[0], sh->env);
	if (!full_path)
		handle_cmd_error(cmd, sh);
	exit_if_directory(cmd, sh, full_path);
	if (!cmd->is_valid)
		handle_cmd_error(cmd, sh);
	reset_child_signals();
	execve(full_path, cmd->argv, sh->env);
	perror(full_path);
	code = get_exec_error_code(full_path);
	free(full_path);
	free_cmd(cmd);
	free(sh->pids);
	free_env_array(sh->env);
	free_minishell(sh);
	exit(code);
}

void	exec_child(t_cmd *cmd, int in_fd, int pipe_fd[2], t_msh *sh)
{
	if (setup_redirections(cmd, sh, in_fd, pipe_fd) == FAILURE)
	{
		if (cmd->redirect_failed && cmd->redirect_failed_path)
		{
			usleep(1000);
			print_redirect_error(cmd);
		}
		exit(1);
	}
	if (!cmd->argv || !cmd->argv[0])
		exec_empty_or_null_cmd(cmd, sh);
	if (cmd->argv[0][0] == '\0' || !cmd->is_valid)
		exec_invalid_cmd(cmd, sh);
	if (cmd->is_builtin)
		exec_builtin_cmd(cmd, sh);
	try_exec_binary(cmd, sh);
	exit(127);
}
