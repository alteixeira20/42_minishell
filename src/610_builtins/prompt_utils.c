/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:48:01 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/04 17:51:39 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*prompt_user(t_minishell *sh)
{
	char	*result;
	char	*temp;

	result = ft_strjoin(sh->user, "@");
	temp = ft_strjoin(result, sh->hostname);
	free(result);
	temp = ft_strtrim(temp, "\n");
	result = ft_strjoin(temp, ":");
	free(temp);
	return (result);
}

char	*build_prompt(t_minishell *sh)
{
	char	*result;
	char	*temp;
	char	*cwd;
	char	*pwd;
	char	*home;

	temp = prompt_user(sh);
	cwd = extract_var("PWD", sh->env);
	if (cwd == NULL)
		cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		home = extract_var("HOME", sh->env);
		if (home == NULL)
			home = ft_strdup(sh->home);
		pwd = build_cwd(cwd, home);
		if (pwd)
			temp = ft_strjoin(temp, pwd);
		free(cwd);
		free(home);
	}
	result = ft_strjoin(temp, "$ ");
	return (free(temp), result);
}

char	*build_cwd(char *cwd, char *home)
{
	char	*result;
	int		i;
	int		cwd_len;
	int		home_len;

	cwd_len = ft_strlen(cwd);
	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) != 0)
		home_len = 0;
	result = ft_calloc((cwd_len - home_len + (home_len > 0) + 1), sizeof(char));
	i = 0;
	if (home_len > 0)
		result[0] = '~';
	while (i < (cwd_len - home_len))
	{
		result[i + (home_len > 0)] = cwd[i + home_len];
		i++;
	}
	result[i + (home_len > 0)] = '\0';
	return (result);
}
