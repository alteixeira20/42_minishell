/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:34:23 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/11 15:11:29 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*get_var_value(const char *name, t_minishell *sh)
{
	if (!ft_strncmp(name, "?", 2))
		return (ft_itoa(sh->exit_status));
	return (extract_var((char *)name, sh->env));
}

static char	*expand_one(const char *str, int *i, t_minishell *sh)
{
	int		start;
	char	*name;
	char	*val;

	start = ++(*i);
	if (str[start] == '?')
		return (++(*i), get_var_value("?", sh));
	while (str[*i] && is_valid_var_char(str[*i]))
		(*i)++;
	name = ft_substr(str, start, *i - start);
	val = get_var_value(name, sh);
	free(name);
	if (!val)
		return (ft_strdup(""));
	return (val);
}

static void	handle_dollar(const char *str, int *i, t_minishell *sh, char **res)
{
	char	*val;
	char	*tmp;

	val = expand_one(str, i, sh);
	tmp = ft_strjoin(*res, val);
	free(*res);
	free(val);
	*res = tmp;
}

char	*expand_token_value(const char *str, t_minishell *sh)
{
	int		i;
	char	quote;
	char	*res;

	i = 0;
	quote = 0;
	res = ft_strdup("");
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i++];
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else if (str[i] == '$' && quote != '\'')
			handle_dollar(str, &i, sh, &res);
		else
			ft_str_append_char(&res, str[i++]);
	}
	return (res);
}
