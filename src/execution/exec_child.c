/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 03:40:27 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/17 15:14:27 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	get_exec_error_code(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
	{
		if (errno == ENOENT)
			return (127);
		else
			return (126);
	}
	if (S_ISDIR(st.st_mode))
		return (126);
	if (access(path, X_OK) != 0)
		return (126);
	return (0);
}

static void	handle_cmd_error(t_cmd *cmd)
{
	struct stat	st;

	if (ft_strchr(cmd->argv[0], '/'))
	{
		if (stat(cmd->argv[0], &st) == 0 && S_ISDIR(st.st_mode))
		{
			ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
			exit(126);
		}
		perror(cmd->argv[0]);
		exit(127);
	}
	ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(127);
}

static void	try_exec_binary(t_cmd *cmd, t_msh *sh)
{
	char	*full_path;
	int		code;

	full_path = get_cmd_path(cmd->argv[0], sh->env);
	if (!full_path)
		handle_cmd_error(cmd);
	reset_child_signals();
	execve(full_path, cmd->argv, sh->env);
	code = get_exec_error_code(full_path);
	perror(full_path);
	free(full_path);
	exit(code);
}

void	exec_child(t_cmd *cmd, int in_fd, int pipe_fd[2], t_msh *sh)
{
	setup_redirections(cmd, in_fd, pipe_fd);
	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		if (cmd->input_fd != STDIN_FILENO || cmd->output_fd != STDOUT_FILENO)
			exit(SUCCESS);
		exit(FAILURE);
	}
	if (cmd->is_builtin)
	{
		reset_child_signals();
		exit(run_builtin(cmd, sh));
	}
	try_exec_binary(cmd, sh);
}
