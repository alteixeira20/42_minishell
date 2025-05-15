/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopedro- <jopedro-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:59:00 by jopedro-          #+#    #+#             */
/*   Updated: 2025/05/15 19:29:54 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_heredoc_line(const char *line, t_msh *sh)
{
	char	*res;
	int		i;

	if (!line)
		return (NULL);
	i = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '$')
			handle_dollar(line, &i, sh, &res);
		else
			ft_str_append_char(&res, line[i++]);
	}
	return (res);
}

char	*handle_expansion(char *line, t_msh *sh)
{
	if (!line)
		return (NULL);
	if (sh->heredoc_quoted)
		return (ft_strdup(line));
	else
		return (expand_heredoc_line(line, sh));
}
