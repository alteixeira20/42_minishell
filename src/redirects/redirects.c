/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:38:08 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/15 00:35:55 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	open_input_file(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		perror(filename);
	return (fd);
}

static int	open_output_file(const char *filename, bool append)
{
	int	fd;

	if (append)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		perror(filename);
	return (fd);
}

static int	handle_redirect_in(t_cmd *cmd, t_token *file_tok)
{
	int	fd;

	if (!file_tok || file_tok->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		return (-1);
	}
	fd = open_input_file(file_tok->value);
	if (fd == -1)
		return (-1);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = fd;
	return (0);
}

static int	handle_redirect_out(t_cmd *cmd, t_token *file_tok, bool append)
{
	int	fd;

	if (!file_tok || file_tok->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		return (-1);
	}
	fd = open_output_file(file_tok->value, append);
	if (fd == -1)
		return (-1);
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	cmd->output_fd = fd;
	return (0);
}

int	process_redirect(t_cmd *cmd, t_token *token)
{
	if (!token || !token->next)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		return (-1);
	}
	if (token->type == TOKEN_REDIRECT_IN)
		return (handle_redirect_in(cmd, token->next));
	else if (token->type == TOKEN_REDIRECT_OUT)
		return (handle_redirect_out(cmd, token->next, false));
	else if (token->type == TOKEN_APPEND)
		return (handle_redirect_out(cmd, token->next, true));
	return (0);
}
