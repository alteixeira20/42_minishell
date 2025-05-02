/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:28:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/30 02:30:21 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_strjoin_split(char *s1, char *s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	ft_memcpy(joined, s1, len1);
	ft_memcpy(joined + len1, s2, len2);
	joined[len1 + len2] = '\0';
	free(s1);
	free(s2);
	return (joined);
}

char	*extract_word(const char *str, int *i)
{
	char	*segment;
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[*i])
	{
		if (ft_isspace(str[*i]) || is_special_char(str[*i]))
			break ;
		segment = get_next_segment(str, i);
		if (!segment)
			break ;
		result = ft_strjoin_split(result, segment);
		if (!result)
			return (NULL);
	}
	return (result);
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
		free(tmp);
	}
	result[count] = NULL;
	return (result);
}
