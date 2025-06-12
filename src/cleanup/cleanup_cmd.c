/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:44:51 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/12 22:56:10 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_redirections(t_redirect *redir)
{
	t_redirect	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->filename);
		free(redir);
		redir = tmp;
	}
}

static void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		printf("[free_argv] Freeing argv[%d]: '%s'\n", i, argv[i]);
		free(argv[i++]);
	}
	free(argv);
}

void	free_one_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_argv(cmd->argv);
	free_redirections(cmd->redirects);
	if (cmd->redirect_failed_path)
		free(cmd->redirect_failed_path);
	free(cmd);
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*temp;
	int n = 0;

	while (cmd)
	{
		temp = cmd->next;
		printf("[free_cmd] Freeing cmd node %d\n", n++);
		free_argv(cmd->argv);
		free_redirections(cmd->redirects);
		if (cmd->redirect_failed_path)
			free(cmd->redirect_failed_path);
		free(cmd);
		cmd = temp;
	}
}
