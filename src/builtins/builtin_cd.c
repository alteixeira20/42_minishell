/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:12:15 by paalexan          #+#    #+#             */
/*   Updated: 2025/07/10 18:11:54 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_pwd(char *oldpwd, t_msh *sh)
{
	char	*newpwd;
	char	*check_oldpwd;
	char	*pwd;

	check_oldpwd = extract_var("OLDPWD", sh->env);
	pwd = extract_var("PWD", sh->env);
	newpwd = getcwd(NULL, 0);
	if (newpwd)
	{
		if (check_oldpwd)
		{
			set_var("OLDPWD", oldpwd, &sh->env);
			free(check_oldpwd);
		}
		if (pwd)
		{
			set_var("PWD", newpwd, &sh->env);
			free(pwd);
		}
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
			return (ft_strdup(sh->home));
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
		return (handle_cd_error(oldpwd, target, sh));
	update_pwd(oldpwd, sh);
	free(oldpwd);
	free(target);
	return (SUCCESS);
}
