/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:28:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/12 18:00:39 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*extract_word(const char *str, int *i)
{
	int		start;
	char	quote;

	start = *i;
	quote = 0;
	while (str[*i])
	{
		if (!quote && (str[*i] == '\'' || str[*i] == '"'))
			quote = str[(*i)++];
		else if (quote && str[*i] == quote)
		{
			(*i)++;
			quote = 0;
		}
		else if (!quote && (ft_isspace(str[*i]) || is_special_char(str[*i])))
			break ;
		else
			(*i)++;
	}
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
