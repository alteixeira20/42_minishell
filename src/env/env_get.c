/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 22:16:44 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/12 21:28:24 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**init_env(char **env)
{
	char	**new_env;
	int		n;

	n = 0;
	while (env[n])
		++n;
	new_env = ft_calloc((n + 1), sizeof(char *));
	if (!new_env)
		return (exit_error(MALLOC_ERR, errno), NULL);
	n = -1;
	while (env[++n])
	{
		new_env[n] = ft_strdup(env[n]);
		if (!new_env[n])
			return (NULL);
	}
	new_env[n] = NULL;
	return (new_env);
}

char	*extract_var(char *var, char **env)
{
	char	*tmp;
	char	*val;
	int		len;

	if ((var == NULL) || (env == NULL))
		return (NULL);
	tmp = ft_strjoin(var, "=");
	len = ft_strlen(tmp);
	val = NULL;
	while (*env)
	{
		if (ft_strncmp(tmp, *env, len) == 0)
		{
			val = ft_strdup(*env + len);
			break ;
		}
		++env;
	}
	free(tmp);
	return (val);
}
