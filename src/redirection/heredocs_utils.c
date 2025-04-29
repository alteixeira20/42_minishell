/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:12:05 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/29 15:30:55 by paalexan         ###   ########.fr       */
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

static int	run_heredoc_child(const char *delim, int fd)
{
	setup_heredoc_signals();
	write_heredoc_content(delim, fd);
	close(fd);
	exit(0);
}

static char	*handle_heredoc_status(int status, char *filename)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(filename);
		free(filename);
		g_exit = 130;
		return (NULL);
	}
	return (filename);
}

char	*write_heredoc_to_tmp(const char *delim, int index)
{
	char	*filename;
	int		fd;
	pid_t	pid;
	int		status;

	filename = heredoc_tmpname(index);
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(filename);
		g_exit = 1;
		return (NULL);
	}
	pid = fork();
	if (pid == 0)
		run_heredoc_child(delim, fd);
	close(fd);
	waitpid(pid, &status, 0);
	return (handle_heredoc_status(status, filename));
}
