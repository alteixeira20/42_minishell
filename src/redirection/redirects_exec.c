/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:35:00 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/24 16:26:50 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_dup2_failure(t_msh *sh)
{
	if (!sh->error_printed)
	{
		perror("dup2");
		sh->error_printed = true;
	}
}

int	redirect_and_close(int fd, int target_fd, t_msh *sh)
{
	if (dup2(fd, target_fd) == -1)
	{
		handle_dup2_failure(sh);
		close(fd);
		g_exit = 1;
		return (FAILURE);
	}
	close(fd);
	return (SUCCESS);
}

int	open_fd_for_redirect(t_redirect *redir)
{
	int	flags;

	if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		return (open(redir->filename, O_RDONLY));
	if (redir->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	return (open(redir->filename, flags, 0644));
}

int	apply_single_redirect(t_cmd *cmd, t_redirect *redir, t_msh *sh)
{
	int	fd;

	fd = open_fd_for_redirect(redir);
	if (fd < 0)
	{
		if (!sh->error_printed)
		{
			sh->error_printed = true;
			cmd->redirect_failed = true;
			cmd->redirect_failed_path = ft_strdup(redir->filename);
		}
		g_exit = 1;
		return (FAILURE);
	}
	if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		return (redirect_and_close(fd, STDIN_FILENO, sh));
	else
		return (redirect_and_close(fd, STDOUT_FILENO, sh));
}
