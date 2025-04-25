/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:49:01 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/25 00:52:33 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[0] || str[0] == '=')
		return (false);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static int	handle_export_args(t_cmd *cmd, t_msh *sh)
{
	int	i;
	int	ret;

	i = 1;
	ret = SUCCESS;
	while (i < cmd->argc)
	{
		if (!is_valid_identifier(cmd->argv[i]))
		{
			export_invalid_identifier(cmd->argv[i]);
			ret = FAILURE;
		}
		else
			export_assign(cmd->argv[i], sh);
		i++;
	}
	return (ret);
}

static void	print_exported_vars(char **env, t_cmd *cmd)
{
	int		i;
	char	*equal;

	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i], "_", 2) == 0)
		{
			i++;
			continue ;
		}
		ft_putstr_fd("declare -x ", cmd->output_fd);
		equal = ft_strchr(env[i], '=');
		if (equal)
		{
			write(cmd->output_fd, env[i], equal - env[i] + 1);
			ft_putchar_fd('"', cmd->output_fd);
			ft_putstr_fd(equal + 1, cmd->output_fd);
			ft_putchar_fd('"', cmd->output_fd);
		}
		else
			ft_putstr_fd(env[i], cmd->output_fd);
		ft_putchar_fd('\n', cmd->output_fd);
		i++;
	}
}

int	cmd_export(t_cmd *cmd, t_msh *sh)
{
	char	**copy;
	int		ret;

	ret = SUCCESS;
	copy = copy_env_array(sh->env);
	if (cmd->argc == 1)
	{
		sort_env(copy);
		print_exported_vars(copy, cmd);
		return (ret);
	}
	else
		ret = handle_export_args(cmd, sh);
	free_env_array(copy);
	g_exit = ret;
	return (ret);
}
