/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:37:36 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/24 12:48:47 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	report_no_permission(const char *filename, t_msh *sh)
{
	if (!sh->error_printed)
	{
		perror(filename);
		sh->error_printed = true;
	}
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

int	check_input_file(const char *filename, t_msh *sh)
{
	if (access(filename, F_OK) == -1 || access(filename, R_OK) == -1)
		return (report_no_permission(filename, sh));
	return (SUCCESS);
}

int	check_output_permission(t_redirect *redir, t_msh *sh)
{
	char	*dir;

	if (access(redir->filename, F_OK) == 0)
	{
		if (access(redir->filename, W_OK) == -1)
			return (report_no_permission(redir->filename, sh));
		return (SUCCESS);
	}
	dir = get_directory(redir->filename);
	if (!dir)
		return (FAILURE);
	if (access(dir, W_OK) == -1)
	{
		free(dir);
		return (report_no_permission(redir->filename, sh));
	}
	free(dir);
	return (SUCCESS);
}
