/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 20:43:21 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/11 23:57:50 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_redirections(t_cmd *cmds, int in_fd, int pipe_fd[2])
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cmds->input_fd != STDIN_FILENO)
	{
		dup2(cmds->input_fd, STDIN_FILENO);
		close(cmds->input_fd);
	}
	if (cmds->output_fd != STDOUT_FILENO)
		dup2(cmds->output_fd, STDOUT_FILENO);
	else if (cmds->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

int	handle_redirect_heredoc(t_cmd *cmd, t_token *file_tok)
{
	int	fd;

	if (!file_tok || file_tok->type != TOKEN_WORD || !file_tok->value)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		return (-1);
	}
	if (cmd->input_fd < 0)
		return (-1);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	fd = process_heredoc(file_tok->value);
	if (fd == -1)
		return (-1);
	cmd->input_fd = fd;
	return (0);
}
