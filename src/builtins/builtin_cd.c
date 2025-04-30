/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:12:15 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/30 01:36:39 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_pwd(char *oldpwd, t_msh *sh)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (newpwd)
	{
		set_var("OLDPWD", oldpwd, &sh->env);
		set_var("PWD", newpwd, &sh->env);
		free(newpwd);
	}
}

static char	*handle_special_targets(char *arg, t_msh *sh)
{
	char	*home;
	char	*expanded;

	if (arg[0] == '~')
	{
		home = extract_var("HOME", sh->env);
		if (!home)
			return (ft_strdup(arg));
		if (arg[1] == '\0')
			return (home);
		expanded = ft_strjoin(home, arg + 1);
		free(home);
		return (expanded);
	}
	if (arg[0] == '-' && arg[1] == '\0')
	{
		home = extract_var("OLDPWD", sh->env);
		if (home)
			printf("%s\n", home);
		return (home);
	}
	if (arg[0] == '\0')
		return (ft_strdup("."));
	return (ft_strdup(arg));
}

static char	*get_target_dir(t_cmd *cmd, t_msh *sh)
{
	char	*dir;

	if (!cmd->argv[1] || ft_strcmp(cmd->argv[1], "--") == 0)
		dir = extract_var("HOME", sh->env);
	else
		dir = handle_special_targets(cmd->argv[1], sh);
	return (dir);
}

static int	handle_cd_error(char *oldpwd, const char *target)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	if (target)
		perror(target);
	else
		ft_putendl_fd("target path is NULL", STDERR_FILENO);
	free(oldpwd);
	g_exit = 1;
	return (FAILURE);
}

int	cmd_cd(t_cmd *cmd, t_msh *sh)
{
	char	*oldpwd;
	char	*target;

	if (!cmd || !cmd->argv || cmd->argc > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		g_exit = 1;
		return (FAILURE);
	}
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		oldpwd = ft_strdup("");
	target = get_target_dir(cmd, sh);
	if (!target || chdir(target) == -1)
		return (handle_cd_error(oldpwd, target));
	update_pwd(oldpwd, sh);
	free(oldpwd);
	free(target);
	return (SUCCESS);
}
