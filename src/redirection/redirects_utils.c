/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 21:08:18 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/18 19:46:35 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_redirect(t_cmd *cmd, t_redirect_type type, const char *filename)
{
	t_redirect	*new;
	t_redirect	*last;

	new = malloc(sizeof(t_redirect));
	if (!new)
		exit_error(MALLOC_ERR, 1);
	new->type = type;
	new->filename = ft_strdup(filename);
	new->next = NULL;
	if (!new->filename)
	{
		free(new);
		exit_error(MALLOC_ERR, 1);
	}
	if (!cmd->redirects)
		cmd->redirects = new;
	else
	{
		last = cmd->redirects;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

int	handle_redirect_in(t_cmd *cmd, t_token *file_tok)
{
	if (!file_tok || file_tok->type != TOKEN_WORD || !file_tok->value)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		return (FAILURE);
	}
	add_redirect(cmd, REDIR_IN, file_tok->value);
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
	if (append)
		add_redirect(cmd, REDIR_APPEND, file_tok->value);
	else
		add_redirect(cmd, REDIR_OUT, file_tok->value);
	return (SUCCESS);
}
