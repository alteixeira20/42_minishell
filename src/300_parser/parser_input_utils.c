/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_input_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:28:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/04 00:21:14 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_special_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

static char	*extract_special_char(const char *str, int *i)
{
	int	start;

	start = *i;
	if ((str[*i] == '>' || str[*i] == '<') && str[*i] == str[*i + 1])
		(*i) += 2;
	else
		(*i) += 1;
	return (ft_substr(str, start, *i - start));
}

static char	*extract_word(const char *str, int *i)
{
	int		start;
	char	quote;

	start = *i;
	quote = 0;
	while (str[*i])
	{
		if ((str[*i] == '\'' || str[*i] == '"') && !quote)
		{
			quote = str[(*i)++];
			while (str[*i] && str[*i] != quote)
				(*i)++;
			if (str[*i] == quote)
				(*i)++;
			quote = 0;
		}
		else if ((ft_isspace(str[*i]) || is_special_char(str[*i])))
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
		result[count++] = tmp;
	}
	result[count] = NULL;
	return (result);
}
