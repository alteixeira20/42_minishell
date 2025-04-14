/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:12:15 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/14 15:44:13 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_pwd_vars(char *oldpwd, t_minishell *sh)
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

int	cmd_cd(t_cmd *cmd, t_minishell *sh)
{
	char	*oldpwd;
	char	*target;

	if (cmd->argv[2][0])
		return (exit_error("minishell: cd: too many arguments\n", 1));
	oldpwd = getcwd(NULL, 0);
	if (!cmd->argv[1] || ft_strncmp(cmd->argv[1], "~", 2) == SUCCESS)
		target = extract_var("HOME", sh->env);
	else if (ft_strncmp(cmd->argv[1], "-", 2) == SUCCESS)
	{
		target = extract_var("OLDPWD", sh->env);
		if (target)
			printf("%s\n", target);
	}
	else if (!cmd->argv[1][0])
		target = ".";
	else
		target = cmd->argv[1];
	if (!target || chdir(target) == -1)
		return (perror("cd"), free(oldpwd), FAILURE);
	update_pwd_vars(oldpwd, sh);
	free(oldpwd);
	return (SUCCESS);
}
