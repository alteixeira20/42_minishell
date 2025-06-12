/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:24:08 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/16 19:03:51 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	res = ft_strjoin(s1, s2);
	free(s1);
	return (res);
}

char	*search_path_in_env(char *cmd, char **paths)
{
	char	*try_path;
	int		i;

	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		try_path = ft_strjoin(paths[i], "/");
		try_path = ft_strjoin_free(try_path, cmd);
		if (access(try_path, X_OK) == 0)
		{
			free_split(paths);
			return (try_path);
		}
		free(try_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

char	*get_cmd_path(char *cmd, char **env)
{
	char	*path_env;
	char	**paths;

	if (!cmd || !env)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = extract_var("PATH", env);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	free (path_env);
	return (search_path_in_env(cmd, paths));
}
