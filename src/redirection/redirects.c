/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:38:08 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/18 19:47:50 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_redirect(t_cmd *cmd, t_token *token, t_msh *sh)
{
	if (!token || !token->next)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		return (FAILURE);
	}
	if (token->type == TOKEN_REDIRECT_IN)
		return (handle_redirect_in(cmd, token->next));
	if (token->type == TOKEN_REDIRECT_OUT)
		return (handle_redirect_out(cmd, token->next, false));
	if (token->type == TOKEN_APPEND)
		return (handle_redirect_out(cmd, token->next, true));
	if (token->type == TOKEN_HEREDOC)
		return (handle_redirect_heredoc(cmd, sh, token->next));
	return (SUCCESS);
}

static void	apply_pipe_redirect(t_cmd *cmd, int pipe_fd[2], t_msh *sh)
{
	if (cmd->next)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1 && !sh->error_printed)
		{
			perror("dup2");
			sh->error_printed = true;
		}
	}
	if (pipe_fd[0] != -1 && pipe_fd[0] != STDOUT_FILENO)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1 && pipe_fd[1] != STDOUT_FILENO)
		close(pipe_fd[1]);
}

static void	apply_fd_redirect(int in_fd, t_msh *sh)
{
	if (dup2(in_fd, STDIN_FILENO) == -1 && !sh->error_printed)
	{
		perror("dup2");
		sh->error_printed = true;
	}
	close(in_fd);
}

void	setup_redirections(t_cmd *cmd, t_msh *sh, int in_fd, int pipe_fd[2])
{
	if (cmd->input_fd == STDIN_FILENO && in_fd != STDIN_FILENO)
		apply_fd_redirect(in_fd, sh);
	else if (in_fd != STDIN_FILENO)
		close(in_fd);
	apply_all_redirects(cmd, sh);
	apply_pipe_redirect(cmd, pipe_fd, sh);
}
