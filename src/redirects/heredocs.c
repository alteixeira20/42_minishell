/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:54:17 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/12 18:04:07 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	setup_heredoc_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

static void	heredoc_child(const char *delim, int pipe_out)
{
	char	*line;

	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_heredoc_warning(delim);
			break ;
		}
		if (!ft_strncmp(line, delim, ft_strlen(delim) + 1) || g_exit == 130)
			break ;
		write(pipe_out, line, ft_strlen(line));
		write(pipe_out, "\n", 1);
		free(line);
	}
	free(line);
	close(pipe_out);
	exit(0);
}

static int	handle_heredoc_status(pid_t pid, int read_end)
{
	int	status;
	int	ret;

	status = 0;
	ret = waitpid(pid, &status, 0);
	while (ret == -1 && errno == EINTR)
		ret = waitpid(pid, &status, 0);
	if (ret == -1)
	{
		if (read_end >= 0)
			close(read_end);
		g_exit = 130;
		return (-1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		if (read_end >= 0)
			close(read_end);
		g_exit = 130;
		return (-1);
	}
	return (0);
}

static pid_t	start_heredoc_child(const char *delim, int pipefd[2])
{
	pid_t	pid;

	if (pipe(pipefd) != 0 || !delim)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_child(delim, pipefd[1]);
	}
	close(pipefd[1]);
	return (pid);
}

int	process_heredoc(const char *delim)
{
	int					pipefd[2];
	pid_t				pid;
	struct sigaction	sa_ignore;
	struct sigaction	sa_old;

	pid = start_heredoc_child(delim, pipefd);
	if (pid == 1)
		return (-1);
	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &sa_old);
	if (handle_heredoc_status(pid, pipefd[0]) == -1)
	{
		sigaction(SIGINT, &sa_old, NULL);
		return (-1);
	}
	sigaction(SIGINT, &sa_old, NULL);
	g_exit = 0;
	return (pipefd[0]);
}
