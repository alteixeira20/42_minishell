/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:54:17 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/16 20:06:59 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_redirect_heredoc(t_cmd *cmd, t_token *file_tok, t_msh *sh)
{
	char	*tmp;

	if (!file_tok || file_tok->type != TOKEN_WORD || !file_tok->value)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		return (-1);
	}
	tmp = write_heredoc_to_tmp(file_tok->value, sh->hd_cnt++);
	if (!tmp)
	{
		cmd->input_fd = -1;
		return (-1);
	}
	free(cmd->input_file);
	cmd->input_file = tmp;
	cmd->input_redirect = true;
	return (0);
}
