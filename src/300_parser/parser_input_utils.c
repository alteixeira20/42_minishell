/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_input_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:28:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/03 03:31:24 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	skip_quoted_word(const char *str, int i)
{
	char	quote;

	quote = str[i];
	i += 1;
	while (str[i] && str[i] != quote)
		i++;
	return (i);
}

static int	count_words(const char *str)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break ;
		counter++;
		while (str[i] && str[i] != ' ')
		{
			if (str[i] == '\'' || str[i] == '\"')
				i = skip_quoted_word(str, i);
			i++;
		}
	}
	return (counter);
}

static char	*word_dup(const char *str, int *i)
{
	int		start;
	char	quote;
	char	*word;

	start = *i;
	while (str[*i] && str[*i] != ' ')
	{
		if (str[*i] == '\'' || str[*i] == '\"')
		{
			quote = str[*i];
			(*i)++;
			while (str[*i] && str[*i] != quote)
				(*i)++;
		}
		(*i)++;
	}
	word = ft_substr(str, start, *i - start);
	if (!word)
		return (NULL);
	return (word);
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

char	**split_input(const char *str)
{
	char	**args;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	args = malloc(sizeof(char *) * (count_words(str) + 1));
	if (!args)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (!str[i])
			break ;
		args[j] = word_dup(str, &i);
		j++;
	}
	args[j] = NULL;
	return (args);
}
