/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:29:39 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/27 20:17:21 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_builtin_cmd(const char *cmd)
{
	if (ft_strcmp(cmd, "env") == 0)
		return (true);
	if (ft_strcmp(cmd, "echo") == 0)
		return (true);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "export") == 0)
		return (true);
	if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (true);
	return (false);
}

void	finalize_cmds(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->argv && cmd->argv[0])
			cmd->is_builtin = is_builtin_cmd(cmd->argv[0]);
		cmd = cmd->next;
	}
}

int	run_builtin(t_cmd *cmd, t_msh *sh)
{
	int	ret;

	if (!cmd || !cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
		return (FAILURE);
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		ret = cmd_env(cmd, sh);
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		ret = cmd_echo(cmd);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		ret = cmd_export(cmd, sh);
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		ret = cmd_cd(cmd, sh);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ret = cmd_exit(cmd, sh);
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		ret = cmd_pwd(cmd);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		ret = cmd_unset(cmd, sh);
	g_exit = ret;
	return (ret);
}
