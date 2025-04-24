/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 03:40:27 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/24 16:58:11 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	get_exec_error_code(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
	{
		if (errno == ENOENT || errno == ENOTDIR)
			return (127);
		return (126);
	}
	if (S_ISDIR(st.st_mode))
		return (126);
	if (access(path, X_OK) != 0)
		return (126);
	return (1);
}

static void	handle_cmd_error(t_cmd *cmd, t_msh *sh)
{
	int		code;
	char	*path;

	path = NULL;
	if (ft_strchr(cmd->argv[0], '/'))
		path = ft_strdup(cmd->argv[0]);
	else
		path = get_cmd_path(cmd->argv[0], sh->env);
	if (path)
	{
		perror(cmd->argv[0]);
		code = get_exec_error_code(path);
		free(path);
	}
	else
	{
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		code = 127;
	}
	exit(code);
}

static void	try_exec_binary(t_cmd *cmd, t_msh *sh)
{
	char		*full_path;
	struct stat	st;
	int			code;

	full_path = get_cmd_path(cmd->argv[0], sh->env);
	if (!full_path)
		handle_cmd_error(cmd, sh);
	if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(full_path, STDERR_FILENO);
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
		free(full_path);
		exit(126);
	}
	reset_child_signals();
	execve(full_path, cmd->argv, sh->env);
	perror(full_path);
	code = get_exec_error_code(full_path);
	free(full_path);
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
	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		if (cmd->input_fd != STDIN_FILENO || cmd->output_fd != STDOUT_FILENO)
			exit(0);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	if (cmd->is_builtin)
	{
		reset_child_signals();
		exit(run_builtin(cmd, sh));
	}
	try_exec_binary(cmd, sh);
	exit(127);
}
