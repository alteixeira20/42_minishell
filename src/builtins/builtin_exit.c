/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 02:22:28 by paalexan          #+#    #+#             */
/*   Updated: 2025/05/26 15:04:43 by jopedro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_too_many_args(t_cmd *cmd, int i)
{
	if (cmd->argv[i + 1])
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd("too many arguments\n", STDERR_FILENO);
		g_exit = 1;
		return (FAILURE);
	}
	else
		return (SUCCESS);
}

static int	handle_not_numeric(char	*arg, long *code)
{
	if (!ft_isnumeric(arg) || !check_valid_long(arg, code))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	handle_arg(t_cmd *cmd, t_msh *sh, int i)
{
	long	code;
	int		exit_code;

	if (handle_too_many_args(cmd, i) == FAILURE)
		return (FAILURE);
	if (handle_not_numeric(cmd->argv[i], &code))
	{
		clean_fds();
		free_cmd(cmd);
		free_env_array(sh->env);
		free_minishell(sh);
		exit(2);
	}
	code = ft_atol(cmd->argv[i]);
	exit_code = (unsigned char)code;
	clean_fds();
	free_cmd(cmd);
	free_env_array(sh->env);
	free_minishell(sh);
	exit(exit_code);
}

int	cmd_exit(t_cmd *cmd, t_msh *sh)
{
	int		i;

	ft_putstr_fd("exit\n", cmd->output_fd);
	i = 1;
	if (cmd->argv[i] && ft_strcmp(cmd->argv[i], "--") == 0)
		i++;
	if (cmd->argv[i])
	{
		if (handle_arg(cmd, sh, i) == FAILURE)
			return (FAILURE);
	}
	clean_fds();
	free_cmd(cmd);
	free_env_array(sh->env);
	free_minishell(sh);
	exit(g_exit);
}
