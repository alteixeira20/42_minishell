/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 22:16:38 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/10 21:47:12 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**env_add_var(char **env, char *new_var)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env && env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env && env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i++] = ft_strdup(new_var);
	new_env[i] = NULL;
	free_env_array(env);
	return (new_env);
}

int	var_from_env(char *var, char **env)
{
	int	i;
	int	len;

	if (!*env)
		return (NO_ENV);
	i = -1;
	len = ft_strlen(var);
	while (env[++i])
	{
		if ((ft_strncmp(var, env[i], len) == SUCCESS)
			&& (env[i][len] == '=' || env[i][len] == '\0'))
			return (i);
	}
	return (NO_VAR);
}

static char	*create_var_str(char *var, char *val)
{
	char	*tmp;
	char	*new;

	if (!val)
		return (ft_strdup(var));
	tmp = ft_strjoin(var, "=");
	if (!tmp)
		return (NULL);
	new = ft_strjoin(tmp, val);
	free(tmp);
	return (new);
}

int	set_var(char *var, char *val, char ***env)
{
	char	*new;
	int		i;

	if (!env | !*env)
		return (NO_ENV);
	new = create_var_str(var, val);
	if (!new)
		return (FAILURE);
	i = var_from_env(var, *env);
	if (i == NO_VAR)
		*env = env_add_var(*env, new);
	else
	{
		free((*env)[i]);
		(*env)[i] = new;
	}
	if (i == NO_VAR)
		free(new);
	return (SUCCESS);
}
