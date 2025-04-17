/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 21:08:18 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/17 17:34:48 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_redirect_in(t_cmd *cmd, t_token *file_tok)
{
	if (!file_tok || file_tok->type != TOKEN_WORD || !file_tok->value)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		cmd->input_file = NULL;
		return (0);
	}
	if (cmd->input_file)
		free(cmd->input_file);
	cmd->input_file = ft_strdup(file_tok->value);
	return (1);
}

int	handle_redirect_out(t_cmd *cmd, t_token *file_tok, bool append)
{
	int	fd;
	int	flags;

	if (!file_tok || file_tok->type != TOKEN_WORD || !file_tok->value)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		cmd->output_fd = -1;
		return (0);
	}
	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(file_tok->value, flags, 0644);
	if (fd < 0)
	{
		perror(file_tok->value);
		cmd->output_fd = -1;
		return (0);
	}
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	cmd->output_fd = fd;
	return (0);
}
