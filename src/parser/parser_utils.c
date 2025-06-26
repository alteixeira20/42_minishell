/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 04:36:27 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 16:01:14 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_dollar_quote(const char *str, int *i, char **res)
{
	int		j;
	char	*val;
	char	*tmp;

	j = *i;
	while (str[j] != '\'' && str[j] != '\"')
		j++;
	val = malloc(sizeof(char) * (j - *i + 1));
	if (!val)
		return ;
	ft_bzero(val, ((j - *i) + 1));
	while (*i < j)
	{
		val[*i - 2] = str[*i];
		(*i)++;
	}
	val[*i - 2] = '\0';
	tmp = ft_strjoin(*res, val);
	free(*res);
	free(val);
	*res = tmp;
}

void	handle_dollar(const char *str, int *i, t_msh *sh, char **res)
{
	char	*val;
	char	*tmp;
	char	*dol;

	dol = (char *)&str[*i + 1];
	if (!ft_isprint(*dol) || (ft_isprint(*dol) && (*dol == '\"'
				|| *dol == '\'' || *dol == ' ')))
	{
		(*i)++;
		tmp = ft_strjoin(*res, "$");
		free(*res);
		*res = tmp;
	}
	else
	{
		val = expand_one(str, i, sh);
		tmp = ft_strjoin(*res, val);
		free(*res);
		free(val);
		*res = tmp;
	}
}

void	handle_single_quote(const char *str, int *i, char **res)
{
	char	*tmp;
	char	*substr;
	int		start;

	start = ++(*i);
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	substr = ft_substr(str, start, *i - start);
	tmp = ft_strjoin(*res, substr);
	free(*res);
	free(substr);
	*res = tmp;
	if (str[*i] == '\'')
		(*i)++;
}

void	handle_double_quote(const char *s, int *i, t_msh *sh, char **res)
{
	(*i)++;
	while (s[*i] && s[*i] != '"')
	{
		if (s[*i] == '$')
			handle_dollar(s, i, sh, res);
		else
		{
			ft_str_append_char(res, s[*i]);
			(*i)++;
		}
	}
	if (s[*i] == '"')
		(*i)++;
}

int	handle_all(const char *str, int *i, t_msh *sh, char **res)
{
	if (str[*i] == '\'')
	{
		handle_single_quote(str, &(*i), &(*res));
		return (1);
	}
	else if (str[*i] == '"')
	{
		handle_double_quote(str, &(*i), sh, &(*res));
		return (2);
	}
	else if (str[*i] == '$' && (str[*i + 1] == '\'' || str[*i + 1] == '\"'))
	{
		(*i) += 2;
		handle_dollar_quote(str, &(*i), &(*res));
		return (5);
	}
	else if (str[*i] == '$' && !sh->is_heredoc)
	{
		handle_dollar(str, &(*i), sh, &(*res));
		return (3);
	}
	else
	{
		ft_str_append_char(&(*res), str[(*i)++]);
		return (4);
	}
}
