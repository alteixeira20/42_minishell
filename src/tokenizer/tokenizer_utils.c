/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:07:31 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 14:09:17 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*unescape_token(const char *str)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
			i++;
		res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

int	is_special_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

char	*extract_special_char(const char *str, int *i)
{
	int		start;
	char	c;
	int		count;

	if (!str || !i || !str[*i])
		return (NULL);
	start = *i;
	c = str[*i];
	count = 0;
	if (c != '<' && c != '>' && c != '|')
		return (NULL);
	while (str[*i] && str[*i] == c)
	{
		(*i)++;
		count++;
	}
	if ((c == '<' || c == '>') && count > 2)
		return (NULL);
	if (c == '|' && count > 1)
		return (NULL);
	return (ft_substr(str, start, *i - start));
}

char	*get_next_segment(const char *str, int *i)
{
	int		start;
	char	quote;

	if (!str[*i])
		return (NULL);
	start = *i;
	if (str[*i] == '"' || str[*i] == '\'')
	{
		quote = str[(*i)++];
		while (str[*i] && str[*i] != quote)
			(*i)++;
		if (str[*i] == quote)
			(*i)++;
	}
	else
	{
		while (str[*i] && !ft_isspace(str[*i])
			&& !is_special_char(str[*i])
			&& str[*i] != '"' && str[*i] != '\'')
			(*i)++;
	}
	if (*i == start)
		return (NULL);
	return (ft_substr(str, start, *i - start));
}
