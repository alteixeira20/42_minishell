/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopedro- <jopedro-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:59:00 by jopedro-          #+#    #+#             */
/*   Updated: 2025/04/29 16:49:08 by jopedro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*join_words(char **tokens)
{
	int		i;
	int		count;
	char	*line;

	i = -1;
	count = 0;
	while (tokens[++i])
		count += ft_strlen(tokens[i]);
	line = malloc(sizeof(char) * count + i);
	if (!line)
		return (NULL);
	i = -1;
	while (tokens[++i])
		ft_strjoin(line, tokens[i]);
	return (line);
}

char	*handle_expansion(char *line, t_msh *sh)
{
	bool	quoted;
	int		i;
	int		j;
	char	**tokens;

	i = -1;
	j = 0;
	while (line[++i])
		if (line[i] == '\'' || line[i] == '"')
			quoted = true;
	if (!quoted)
	{
		tokens = ft_split(line, ' ');
		i = -1;
		while (tokens[++i])
		{
			if (tokens[i][0] == '$')
				tokens[i] = expand_one(tokens[i], &j, sh);
		}
	}
	return (join_words(tokens));
}
