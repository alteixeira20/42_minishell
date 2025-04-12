/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:19:32 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/12 21:43:22 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_heredoc_warning(const char *delim)
{
	ft_putstr_fd("warning: here-document at line 1 ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd((char *)delim, STDERR_FILENO);
	ft_putstr_fd("'=\n", STDERR_FILENO);
}

int	check_syntax(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_PIPE)
		return (ft_putendl_fd("syntax error near unexpected token `|'", 2), 1);
	while (tokens->next)
	{
		if (tokens->type == TOKEN_PIPE
			&& tokens->next->type == TOKEN_PIPE)
			return (ft_putendl_fd("syntax error near unexpected token `|'", 2), 1);
		tokens = tokens->next;
	}
	if (tokens->type == TOKEN_PIPE)
		return (ft_putendl_fd("syntax error: unexpected end of file", 2), 1);
	return (0);
}

int	exit_error(char *msg, int status)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	g_exit = status;
	return (FAILURE);
}
