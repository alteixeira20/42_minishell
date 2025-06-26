/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:54:17 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 13:16:49 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_redirect_heredoc(t_cmd *cmd, t_msh *sh, t_token *file_tok)
{
	char	*filename;

	if (!file_tok || file_tok->type != TOKEN_WORD || !file_tok->value)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		return (FAILURE);
	}
	filename = write_heredoc_to_tmp(file_tok->value, sh->hd_cnt++, sh, cmd);
	if (!filename)
		return (FAILURE);
	add_redirect(cmd, REDIR_HEREDOC, filename);
	return (SUCCESS);
}
