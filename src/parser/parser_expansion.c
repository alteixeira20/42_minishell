/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:34:23 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/12 21:04:58 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*get_var_value(const char *name, t_minishell *sh)
{
	char	*raw;

	raw = extract_var((char *)name, sh->env);
	if (!ft_strncmp(name, "?", 2))
		return (ft_itoa(sh->exit_status));
	if (!raw)
		return (NULL);
	return (ft_strdup(raw));
}

char	*expand_one(const char *str, int *i, t_minishell *sh)
{
	int		start;
	char	*name;
	char	*val;

	start = ++(*i);
	if (str[start] == '?')
		return (++(*i), get_var_value("?", sh));
	if (!ft_isalpha(str[start]) && str[start] != '_')
	{
		(*i)++;
		return (ft_strdup(""));
	}
	while (str[*i] && is_valid_var_char(str[*i]))
		(*i)++;
	name = ft_substr(str, start, *i - start);
	val = get_var_value(name, sh);
	free(name);
	if (!val)
		return (ft_strdup(""));
	return (val);
}

static char	*expand_core(const char *str, t_minishell *sh)
{
	int		i;
	char	*res;

	i = 0;
	res = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '\'')
			handle_single_quote(str, &i, &res);
		else if (str[i] == '"')
			handle_double_quote(str, &i, sh, &res);
		else if (str[i] == '$')
			handle_dollar(str, &i, sh, &res);
		else
			ft_str_append_char(&res, str[i++]);
	}
	return (res);
}

char	*expand_token_value(const char *str, t_minishell *sh)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
		return (ft_strdup(str));
	return (expand_core(str, sh));
}
