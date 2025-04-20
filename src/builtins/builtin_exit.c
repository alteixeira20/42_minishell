/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 02:22:28 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/19 21:29:56 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_not_numeric(char	*arg)
{
	if (!ft_isnumeric(arg))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	cmd_exit(t_cmd *cmd, t_msh *sh)
{
	long	code;
	int		exit_code;

	ft_putstr_fd("exit\n", cmd->output_fd);
	if (cmd->argv[1])
	{
		if (cmd->argv[2])
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd("too many arguments\n", STDERR_FILENO);
			g_exit = 1;
			return (FAILURE);
		}
		if (handle_not_numeric(cmd->argv[1]))
		{
			free_minishell(sh);
			exit(2);
		}
		code = ft_atol(cmd->argv[1]);
		exit_code = (unsigned char)code;
		free_minishell(sh);
		exit(exit_code);
	}
	free_minishell(sh);
	exit(g_exit);
}
