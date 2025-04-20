/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:35:00 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/19 20:46:39 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_open_failure(const char *filename, t_msh *sh)
{
	if (!sh->error_printed)
	{
		perror(filename);
		sh->error_printed = true;
	}
	g_exit = 1;
	exit(FAILURE);
}

static void	handle_dup2_failure(t_msh *sh)
{
	if (!sh->error_printed)
	{
		perror("dup2");
		sh->error_printed = true;
	}
}

void	open_last_input(const char *last_in, t_msh *sh)
{
	int	fd;

	if (access(last_in, F_OK) == 0 && access(last_in, R_OK) == -1)
		handle_open_failure(last_in, sh);
	fd = open(last_in, O_RDONLY);
	if (fd < 0)
		handle_open_failure(last_in, sh);
	if (dup2(fd, STDIN_FILENO) == -1)
		handle_dup2_failure(sh);
	close(fd);
}

void	open_last_output(const char *last_out, bool append, t_msh *sh)
{
	int	fd;
	int	flags;

	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	if (access(last_out, F_OK) == 0 && access(last_out, W_OK) == -1)
		handle_open_failure(last_out, sh);
	fd = open(last_out, flags, 0644);
	if (fd < 0)
		handle_open_failure(last_out, sh);
	if (dup2(fd, STDOUT_FILENO) == -1)
		handle_dup2_failure(sh);
	close(fd);
}
