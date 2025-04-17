/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:12:05 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/16 19:59:08 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*heredoc_tmpname(int index)
{
	char	*base;
	char	*num;
	char	*res;

	base = ft_strdup("/tmp/.minishell_heredoc_");
	num = ft_itoa(index);
	if (!base || !num)
	{
		free(base);
		free(num);
		return (NULL);
	}
	res = ft_strjoin(base, num);
	free(base);
	free(num);
	return (res);
}

static void	write_heredoc_content(const char *delim, int fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || g_exit == 130
			|| ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
			break ;
		ft_putendl_fd(line, fd);
		free(line);
	}
	free(line);
}

char	*write_heredoc_to_tmp(const char *delim, int index)
{
	char	*filename;
	int		fd;

	filename = heredoc_tmpname(index);
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(filename);
		return (NULL);
	}
	setup_heredoc_signals();
	write_heredoc_content(delim, fd);
	close(fd);
	return (filename);
}
