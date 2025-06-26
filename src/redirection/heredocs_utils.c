/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:12:05 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 17:11:57 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	add_heredoc_tmpfile(t_hdoc_tmpfile *new_node, t_msh *sh)
{
	new_node->next = sh->heredoc_tmpfiles;
	new_node->prev = NULL;
	if (sh->heredoc_tmpfiles)
		sh->heredoc_tmpfiles->prev = new_node;
	sh->heredoc_tmpfiles = new_node;
}

static char	*heredoc_tmpname(int index, t_msh *sh)
{
	char			*base;
	char			*num;
	char			*res;
	t_hdoc_tmpfile	*new_node;

	base = ft_strdup("/tmp/.minishell_heredoc_");
	num = ft_itoa(index);
	if (!base || !num)
		return (free(base), free(num), NULL);
	res = ft_strjoin(base, num);
	free(base);
	free(num);
	new_node = malloc(sizeof(t_hdoc_tmpfile));
	if (!new_node)
		return (res);
	new_node->path = res;
	add_heredoc_tmpfile(new_node, sh);
	return (res);
}

static void	write_heredoc_content(const char *delim, int fd, t_msh *sh)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_exit != 130)
				print_heredoc_warning(delim);
			else
				ft_putstr_fd("\n", STDOUT_FILENO);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		line = handle_expansion(line, sh);
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static int	run_heredoc_child(const char *delim, int fd,
							t_msh *sh, t_cmd *cmds)
{
	setup_heredoc_signals();
	write_heredoc_content(delim, fd, sh);
	close(fd);
	free_cmd(cmds);
	free_env_array(sh->env);
	free_hc_minishell(sh);
	exit(0);
}

char	*write_heredoc_to_tmp(const char *delim, int index,
							t_msh *sh, t_cmd *cmds)
{
	char			*filename;
	int				fd;
	pid_t			pid;
	int				status;

	filename = heredoc_tmpname(index, sh);
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
		run_heredoc_child(delim, fd, sh, cmds);
	close(fd);
	waitpid(pid, &status, 0);
	return (handle_heredoc_status(status, filename));
}
