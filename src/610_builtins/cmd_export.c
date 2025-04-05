/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:49:01 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/05 15:24:11 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (false);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static void	export_invalid_identifier(const char *arg)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	g_exit = 1;
}

static void	export_assign(const char *arg, t_minishell *sh)
{
	char	*equal;
	char	*key;
	char	*val;

	equal = ft_strchr(arg, '=');
	if (!equal)
	{
		key = ft_substr(arg, 0, equal - arg);
		val = ft_strdup(equal + 1);
	}
	else
	{
		key = ft_strdup(arg);
		val = NULL;
	}
	if (key && val)
		set_var(key, val, &sh->env);
	free(key);
	free(val);
}

static void	print_exported_vars(char **env)
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
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		equal = ft_strchr(env[i], '=');
		if (equal)
		{
			write(STDOUT_FILENO, env[i], equal - env[i] + 1);
			ft_putchar_fd('"', STDOUT_FILENO);
			ft_putstr_fd(equal + 1, STDOUT_FILENO);
			ft_putchar_fd('"', STDOUT_FILENO);
		}
		else
			ft_putstr_fd(env[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}

int	cmd_export(t_cmd *cmd, t_minishell *sh)
{
	int		i;
	char	**copy;

	copy = copy_env_array(sh->env);
	if (cmd->argc == 1)
	{
		sort_env(copy);
		print_exported_vars(copy);
		free_env_array(copy);
		return (SUCCESS);
	}
	i = 1;
	while (i < cmd->argc)
	{
		if (!is_valid_identifier(cmd->argv[i]))
			export_invalid_identifier(cmd->argv[i]);
		else
			export_assign(cmd->argv[i], sh);
		i++;
	}
	if (cmd->argc > 1)
		set_var("_", cmd->argv[cmd->argc - 1], &sh->env);
	return (SUCCESS);
}
