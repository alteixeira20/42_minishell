/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_apply.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:44:18 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/20 04:52:59 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int	check_file_permissions(t_redirect *redir, t_msh *sh)
{
	char	*dir;

	if (access(redir->filename, F_OK) == 0)
	{
		if (access(redir->filename, W_OK) == -1)
		{
			if (!sh->error_printed)
			{
				perror(redir->filename);
				sh->error_printed = true;
			}
			g_exit = 1;
			return (FAILURE);
		}
	}
	else
	{
		dir = get_directory(redir->filename);
		if (!dir)
			return (FAILURE);
		if (access(dir, W_OK) == -1)
		{
			if (!sh->error_printed)
			{
				perror(redir->filename);
				sh->error_printed = true;
			}
			free(dir);
			g_exit = 1;
			return (FAILURE);
		}
		free(dir);
	}
	return (SUCCESS);
}

static void	open_one_output(t_redirect *redir, t_msh *sh)
{
	int	fd;
	int	flags;

	if (check_file_permissions(redir, sh) == FAILURE)
		return ;
	if (redir->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir->filename, flags, 0644);
	if (fd >= 0)
		close(fd);
	else if (!sh->error_printed)
	{
		perror(redir->filename);
		sh->error_printed = true;
	}
}

void	open_all_outputs(t_cmd *cmd, t_msh *sh, char **last_out, bool *append)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			open_one_output(redir, sh);
			*last_out = redir->filename;
			if (redir->type == REDIR_APPEND)
				*append = true;
			else
				*append = false;
		}
		redir = redir->next;
	}
}

static void	collect_last_redirects(t_redirect *redir, char **last_in)
{
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
			*last_in = redir->filename;
		redir = redir->next;
	}
}

static int	check_input_file(const char *filename, t_msh *sh)
{
	if (access(filename, F_OK) == -1)
	{
		if (!sh->error_printed)
		{
			perror(filename);
			sh->error_printed = true;
		}
		g_exit = 1;
		return (FAILURE);
	}
	if (access(filename, R_OK) == -1)
	{
		if (!sh->error_printed)
		{
			perror(filename);
			sh->error_printed = true;
		}
		g_exit = 1;
		return (FAILURE);
	}
	return (SUCCESS);
}

int	apply_all_redirects(t_cmd *cmd, t_msh *sh)
{
	char		*last_in;
	char		*last_out;
	bool		append;

	last_in = NULL;
	last_out = NULL;
	append = false;
	collect_last_redirects(cmd->redirects, &last_in);
	open_all_outputs(cmd, sh, &last_out, &append);
	if (last_in && check_input_file(last_in, sh) == SUCCESS)
		open_last_input(last_in, sh);
	if (last_out)
		open_last_output(last_out, append, sh);
	if (sh->error_printed)
		return (FAILURE);
	return (SUCCESS);
}
