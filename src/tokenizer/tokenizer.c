/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:28:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/12 00:49:30 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	advance_inside_quotes(const char *str, int *i)
{
	char	quote;

	quote = str[(*i)++];
	while (str[*i] && str[*i] != quote)
	{
		if (quote == '"' && str[*i] == '\\' && str[*i + 1])
			(*i)++;
		(*i)++;
	}
	if (str[*i] == quote)
		(*i)++;
}

static void	advance_word_bounds(const char *str, int *i)
{
	int		escaped;

	escaped = 0;
	while (str[*i])
	{
		if (!escaped && str[*i] == '\\')
		{
			escaped = 1;
			if (str[++(*i)])
				(*i)++;
			continue ;
		}
		if (!escaped && (str[*i] == '\'' || str[*i] == '"'))
		{
			advance_inside_quotes(str, i);
			continue ;
		}
		else if (!escaped && (ft_isspace(str[*i]) || is_special_char(str[*i])))
			break ;
		else
			(*i)++;
		escaped = 0;
	}
}

static char	*extract_word(const char *str, int *i)
{
	int	start;

	start = *i;
	advance_word_bounds(str, i);
	return (ft_substr(str, start, *i - start));
}

static char	*get_next_token(const char *str, int *i)
{
	if (is_special_char(str[*i]))
		return (extract_special_char(str, i));
	return (extract_word(str, i));
}

char	**split_input(const char *str)
{
	char	**result;
	int		i;
	int		count;
	char	*tmp;

	if (!str)
		return (NULL);
	result = ft_calloc(ft_strlen(str) + 1, sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	count = 0;
	while (str[i])
	{
		while (ft_isspace(str[i]))
			i++;
		if (!str[i])
			break ;
		tmp = get_next_token(str, &i);
		result[count++] = unescape_token(tmp);
	}
	result[count] = NULL;
	return (result);
}
