/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 21:08:18 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/18 15:01:51 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_redirect_in(t_cmd *cmd, t_token *file_tok)
{
	if (!file_tok || file_tok->type != TOKEN_WORD || !file_tok->value)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		return (FAILURE);
	}
	free(cmd->input_file);
	cmd->input_file = ft_strdup(file_tok->value);
	cmd->input_redirect = true;
	return (SUCCESS);
}

int	handle_redirect_out(t_cmd *cmd, t_token *file_tok, bool append)
{
	if (!file_tok || file_tok->type != TOKEN_WORD || !file_tok->value)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		return (FAILURE);
	}
	free(cmd->output_file);
	cmd->output_file = ft_strdup(file_tok->value);
	cmd->output_redirect = true;
	cmd->append_out = append;
	return (SUCCESS);
}
