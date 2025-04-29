/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:34:23 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/29 19:47:34 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*get_var_value(const char *name, t_msh *sh)
{
	char	*raw;

	if (!ft_strcmp(name, "?"))
		return (ft_itoa(g_exit));
	raw = extract_var((char *)name, sh->env);
	if (!raw)
		return (NULL);
	return (ft_strdup(raw));
}

char	*expand_one(const char *str, int *i, t_msh *sh)
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

static char	*expand_core(const char *str, t_msh *sh, bool *expanded)
{
	int		i;
	char	*res;

	i = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'')
			handle_single_quote(str, &i, &res);
		else if (str[i] == '"')
			handle_double_quote(str, &i, sh, &res);
		else if (str[i] == '$')
		{
			handle_dollar(str, &i, sh, &res);
			*expanded = true;
		}
		else
			ft_str_append_char(&res, str[i++]);
	}
	return (res);
}

char	*expand_token(const char *val, t_msh *sh, bool *quoted, bool *expanded)
{
	size_t	len;
	int		i;

	*quoted = false;
	*expanded = false;
	if (!val)
		return (NULL);
	len = ft_strlen(val);
	if (len >= 2 && val[0] == '\'' && val[len - 1] == '\'')
	{
		*quoted = true;
		return (ft_substr(val, 1, len - 2));
	}
	i = 0;
	while (val[i])
	{
		if (val[i] == '\'' || val[i] == '"')
		{
			sh->heredoc_quoted = true;
			break ;
		}
		i++;
	}
	return (expand_core(val, sh, expanded));
}
