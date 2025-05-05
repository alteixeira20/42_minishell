/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_apply.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:44:18 by paalexan          #+#    #+#             */
/*   Updated: 2025/05/06 00:36:52 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_read_redirect(t_redirect_type type)
{
	return (type == REDIR_IN || type == REDIR_HEREDOC);
}

static int	is_write_redirect(t_redirect_type type)
{
	return (type == REDIR_OUT || type == REDIR_APPEND);
}

static int	validate_redirect(t_cmd *cmd, t_redirect *redir)
{
	if (is_read_redirect(redir->type))
		return (check_input_file(cmd, redir->filename));
	else if (is_write_redirect(redir->type))
		return (check_output_permission(cmd, redir));
	return (SUCCESS);
}

int	apply_all_redirects(t_cmd *cmd, t_msh *sh)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		if (validate_redirect(cmd, redir) == FAILURE)
		{
			cmd->redirect_failed = true;
			if (!cmd->redirect_failed_path)
				cmd->redirect_failed_path = ft_strdup(redir->filename);
			g_exit = 1;
			return (FAILURE);
		}
		if (apply_single_redirect(cmd, redir, sh) == FAILURE)
		{
			cmd->redirect_failed = true;
			if (!cmd->redirect_failed_path)
				cmd->redirect_failed_path = ft_strdup(redir->filename);
			return (FAILURE);
		}
		redir = redir->next;
	}
	return (SUCCESS);
}
