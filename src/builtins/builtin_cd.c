/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:12:15 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/17 01:28:38 by paalexan         ###   ########.fr       */
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
	char	*oldpwd;

	if (arg[0] == '~' && arg[1] == '\0')
		return (extract_var("HOME", sh->env));
	if (arg[0] == '-' && arg[1] == '\0')
	{
		oldpwd = extract_var("OLDPWD", sh->env);
		if (oldpwd)
			printf("%s\n", oldpwd);
		return (oldpwd);
	}
	if (arg[0] == '\0')
		return (".");
	return (arg);
}

static char	*get_target_dir(t_cmd *cmd, t_msh *sh)
{
	char	*dir;

	if (!cmd->argv[1])
		dir = extract_var("HOME", sh->env);
	dir = handle_special_targets(cmd->argv[1], sh);
	return (dir);
}

static int	handle_cd_error(char *oldpwd, const char *target)
{
	perror(ft_strjoin("cd: ", target));
	free(oldpwd);
	if (!target)
		ft_putstr_fd("minishell: cd: target path is NULL\n", STDERR_FILENO);
	return (FAILURE);
}

int	cmd_cd(t_cmd *cmd, t_msh *sh)
{
	char	*oldpwd;
	char	*target;

	if (!cmd || !cmd->argv || cmd->argc > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
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
	return (SUCCESS);
}
