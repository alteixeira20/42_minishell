/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 00:42:57 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/30 20:44:43 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	export_invalid_identifier(const char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	g_exit = 1;
}

static void	export_append(const char *arg, t_msh *sh)
{
	char	*key;
	char	*val;
	char	*existing;
	char	*new_val;

	key = ft_substr(arg, 0, ft_strnstr(arg, "+=", ft_strlen(arg)) - arg);
	val = ft_strdup(ft_strnstr(arg, "+=", ft_strlen(arg)) + 2);
	existing = extract_var(key, sh->env);
	if (existing)
		new_val = ft_strjoin(existing, val);
	else
		new_val = ft_strdup(val);
	set_var(key, new_val, &sh->env);
	free(key);
	free(val);
	free(new_val);
}

static void	export_replace(const char *arg, t_msh *sh)
{
	char	*key;
	char	*val;
	char	*equal;

	key = NULL;
	val = NULL;
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		key = ft_substr(arg, 0, equal - arg);
		val = ft_strdup(equal + 1);
		set_var(key, val, &sh->env);
	}
	else
	{
		key = ft_strdup(arg);
		if (!extract_var(key, sh->env))
			set_var(key, NULL, &sh->env);
	}
	free(val);
	free(key);
}

void	export_assign(const char *arg, t_msh *sh)
{
	if (ft_strnstr(arg, "+=", ft_strlen(arg)))
		export_append(arg, sh);
	else
		export_replace(arg, sh);
}
