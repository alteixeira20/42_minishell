/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 02:22:28 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 17:02:27 by paalexan         ###   ########.fr       */
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

static int	parse_sign_and_whitespace(const char *str, int *i)
{
	int	sign;

	*i = 0;
	sign = 1;
	while (str[*i] == ' ' || (str[*i] >= 9 && str[*i] <= 13))
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static int	check_valid_long(const char *str, long *code)
{
	int				i;
	int				sign;
	long			result;

	result = 0;
	sign = parse_sign_and_whitespace(str, &i);
	if (!str[i])
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if ((sign == 1 && result > (LONG_MAX - (str[i] - '0')) / 10)
			|| (sign == -1 && (unsigned long)result
				> ((unsigned long)LONG_MAX + 1 - (str[i] - '0')) / 10))
			return (0);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (0);
	if (sign == 1)
		*code = result;
	else
		*code = result * sign;
	return (1);
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

int	cmd_exit(t_cmd *cmd, t_msh *sh)
{
	long	code;
	int		exit_code;
	int		i;

	ft_putstr_fd("exit\n", cmd->output_fd);
	i = 1;
	if (cmd->argv[i] && ft_strcmp(cmd->argv[i], "--") == 0)
		i++;
	if (cmd->argv[i])
	{
		if (handle_not_numeric(cmd->argv[i], &code))
		{
			clean_all(sh, cmd);
			exit(2);
		}
		if (handle_too_many_args(cmd, i) == FAILURE)
			return (FAILURE);
		code = ft_atol(cmd->argv[i]);
		exit_code = (unsigned char)code;
		clean_all(sh, cmd);
		exit(exit_code);
	}
	clean_all(sh, cmd);
	exit(g_exit);
}
