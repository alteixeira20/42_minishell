/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:16:54 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/24 12:20:50 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_valid_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

static void	print_echo_args(t_cmd *cmd, int i)
{
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], cmd->output_fd);
		if (cmd->argv[i + 1])
			ft_putchar_fd(' ', cmd->output_fd);
		i++;
	}
}

int	cmd_echo(t_cmd *cmd)
{
	int		i;
	bool	new_line;

	i = 1;
	new_line = true;
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	while (cmd->argv[i] && is_valid_n_flag(cmd->argv[i]))
	{
		new_line = false;
		i++;
	}
	print_echo_args(cmd, i);
	if (new_line)
		ft_putchar_fd('\n', cmd->output_fd);
	return (0);
}
