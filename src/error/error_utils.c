/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:45:18 by paalexan          #+#    #+#             */
/*   Updated: 2025/07/10 18:13:21 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_exec_error_code(char *path)
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

void	handle_cmd_error(t_cmd *cmd, t_msh *sh)
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
		free(sh->pids);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		code = 127;
	}
	free_cmd(cmd);
	free_env_array(sh->env);
	free_hc_minishell(sh);
	exit(code);
}

void	handle_redirection_failure(t_cmd *cmd)
{
	if (cmd->redirect_failed && cmd->redirect_failed_path)
	{
		usleep(1000);
		print_redirect_error(cmd);
	}
	exit(1);
}

static void	print_cd_error_message(char *target, char *home)
{
	if (target)
	{
		if (target[0] == '-')
			ft_putendl_fd("invalid option", STDERR_FILENO);
		else
			perror(target);
	}
	else
	{
		if (home)
			ft_putendl_fd("target path is NULL", STDERR_FILENO);
		else
			ft_putendl_fd("HOME not set", STDERR_FILENO);
	}
}

int	handle_cd_error(char *oldpwd, char *target, t_msh *sh)
{
	char	*home;

	home = extract_var("HOME", sh->env);
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	g_exit = 2;
	print_cd_error_message(target, home);
	free(oldpwd);
	free(target);
	free(home);
	return (g_exit);
}
