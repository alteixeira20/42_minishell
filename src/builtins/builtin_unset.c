/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopedro- <jopedro-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:22:16 by jopedro-          #+#    #+#             */
/*   Updated: 2025/05/12 18:07:59 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

static int	count_env_size(char **env)
{
	int	count;

	count = 0;
	while (env && env[count])
		count++;
	return (count);
}

static int	var_index(char *key, char **env)
{
	int	i;
	int	len;

	if (!key || !env)
		return (-1);
	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && (env[i][len] == '=' || env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

static char	**del_var(char **env, char *key)
{
	int		i;
	int		j;
	int		index;
	char	**new_env;

	index = var_index(key, env);
	if (index == -1)
		return (env);
	new_env = malloc(sizeof(char *) * count_env_size(env));
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (i != index)
			new_env[j++] = ft_strdup(env[i]);
		free(env[i]);
		i++;
	}
	new_env[j] = NULL;
	free(env);
	return (new_env);
}

int	cmd_unset(t_cmd *cmd, t_msh *sh)
{
	char	*arg;
	int		i;

	i = 0;
	if (!sh->env)
		return (SUCCESS);
	if (cmd->argv[1] && cmd->argv[1][0] == '-' && cmd->argv[1][1] != '\0')
		return (exit_error("flag error", 1));
	while (cmd->argv[++i])
	{
		arg = cmd->argv[i];
		if (ft_strchr(arg, '=') == NULL)
			if (var_index(arg, sh->env) >= 0)
				sh->env = del_var(sh->env, arg);
	}
	return (SUCCESS);
}
