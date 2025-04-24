/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:37:36 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/24 16:13:03 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	report_no_permission(t_cmd *cmd, const char *filename)
{
	cmd->redirect_failed = true;
	if (!cmd->redirect_failed_path)
		cmd->redirect_failed_path = ft_strdup(filename);
	g_exit = 1;
	return (FAILURE);
}

static char	*get_directory(const char *filepath)
{
	int		len;
	char	*dir;

	len = ft_strlen(filepath);
	while (len > 0 && filepath[len] != '/')
		len--;
	if (len == 0)
		return (ft_strdup("."));
	dir = ft_substr(filepath, 0, len);
	return (dir);
}

int	check_input_file(t_cmd *cmd, const char *filename)
{
	if (access(filename, F_OK) == -1 || access(filename, R_OK) == -1)
		return (report_no_permission(cmd, filename));
	return (SUCCESS);
}

int	check_output_permission(t_cmd *cmd, t_redirect *redir)
{
	char	*dir;

	if (access(redir->filename, F_OK) == 0)
	{
		if (access(redir->filename, W_OK) == -1)
			return (report_no_permission(cmd, redir->filename));
		return (SUCCESS);
	}
	dir = get_directory(redir->filename);
	if (!dir)
		return (FAILURE);
	if (access(dir, W_OK) == -1)
	{
		free(dir);
		return (report_no_permission(cmd, redir->filename));
	}
	free(dir);
	return (SUCCESS);
}
