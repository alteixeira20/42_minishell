/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:38:08 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/17 17:32:35 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_redirect(t_cmd *cmd, t_token *token, t_msh *sh)
{
	if (!token || !token->next)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		return (-1);
	}
	if (token->type == TOKEN_REDIRECT_IN)
		return (handle_redirect_in(cmd, token->next));
	if (token->type == TOKEN_REDIRECT_OUT)
		return (handle_redirect_out(cmd, token->next, false));
	if (token->type == TOKEN_APPEND)
		return (handle_redirect_out(cmd, token->next, true));
	if (token->type == TOKEN_HEREDOC)
		return (handle_redirect_heredoc(cmd, token->next, sh));
	return (0);
}

static void	apply_input_redirect(t_cmd *cmd)
{
	if (cmd->input_file && cmd->input_fd == STDIN_FILENO)
	{
		cmd->input_fd = open(cmd->input_file, O_RDONLY);
		if (cmd->input_fd < 0)
		{
			perror(cmd->input_file);
			exit(1);
		}
	}
	if (cmd->input_fd != STDIN_FILENO && cmd->input_fd != -1)
	{
		if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
			perror("dup2");
		close(cmd->input_fd);
	}
}

static void	apply_output_redirect(t_cmd *cmd)
{
	int	fd;
	int	flags;

	if (!cmd->output_redirect || !cmd->output_file)
		return ;
	if (cmd->append_out)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(cmd->output_file, flags, 0644);
	if (fd < 0)
	{
		perror(cmd->output_file);
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(fd);
}

static void	apply_pipe_redirect(t_cmd *cmd, int pipe_fd[2])
{
	if (cmd->next && !cmd->output_redirect)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			perror("dup2");
	}
	if (pipe_fd[0] != -1 && pipe_fd[0] != STDOUT_FILENO)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1 && pipe_fd[1] != STDOUT_FILENO)
		close(pipe_fd[1]);
}

void	setup_redirections(t_cmd *cmd, int in_fd, int pipe_fd[2])
{
	if (cmd->input_fd == STDIN_FILENO && in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			perror("dup2");
		close(in_fd);
	}
	else if (in_fd != STDIN_FILENO)
		close(in_fd);
	apply_input_redirect(cmd);
	apply_output_redirect(cmd);
	apply_pipe_redirect(cmd, pipe_fd);
}
