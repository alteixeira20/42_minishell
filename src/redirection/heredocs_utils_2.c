/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopedro- <jopedro-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:59:00 by jopedro-          #+#    #+#             */
/*   Updated: 2025/04/29 18:18:01 by paalexan         ###   ########.fr       */
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

char	*handle_expansion(char *line, t_cmd *cmds, t_msh *sh)
{
	bool	quoted;
	int		i;
	int		j;

	quoted = false;
	if (!line)
		return (NULL);
	i = -1;
	while (cmds->argv[++i])
	{
		if (ft_strcmp(cmds->argv[i], "<<") == 0)
			break ;
	}
	j = 0;
	//printf("cmds->argv[%i][%i] = [%s]", i, j, cmds->argv[i][j]);
	printf("cmds->argv[%i] = [%s]\n", i, cmds->argv[i]);
	while (cmds->argv[i][j])
	{
		printf("cmds->argv[%i] = [%s]\n", i, cmds->argv[i]);
		if (cmds->argv[i][j] == '\'' || cmds->argv[i][j] == '"')
		{
			quoted = true;
			break ;
		}
		j++;
	}
	if (quoted)
		return (ft_strdup(line));
	else
		return (expand_heredoc_line(line, sh));
}
