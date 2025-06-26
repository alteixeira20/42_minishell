/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_invalid.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:51:35 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 16:56:25 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_empty_or_null_cmd(t_cmd *cmd, t_msh *sh)
{
	if (!cmd->next)
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	free_cmd(cmd);
	free_env_array(sh->env);
	free_minishell(sh);
	if (cmd->next)
		exit (0);
	else
		exit (127);
}

void	exec_invalid_cmd(t_cmd *cmd, t_msh *sh)
{
	free_cmd(cmd);
	free_env_array(sh->env);
	free_minishell(sh);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(127);
}
