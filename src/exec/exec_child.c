/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 03:40:27 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/15 02:30:20 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static void	try_exec_binary(t_cmd *cmd, t_minishell *sh)
{
	char	*full_path;

	full_path = get_cmd_path(cmd->argv[0], sh->env);
	if (!full_path)
		handle_cmd_error(cmd);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	execve(full_path, cmd->argv, sh->env);
	perror(full_path);
	free(full_path);
	exit(127);
}

void	exec_child(t_cmd *cmd, int in_fd, int pipe_fd[2], t_minishell *sh)
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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_DFL);
		exit(run_builtin(cmd, sh));
	}
	try_exec_binary(cmd, sh);
}
