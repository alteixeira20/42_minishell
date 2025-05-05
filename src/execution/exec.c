/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:31:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/05/05 18:02:11 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_commands(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	execute_all(t_cmd *cmds, t_msh *sh, int *in_fd, pid_t *pids)
{
	int		i;
	t_cmd	*tmp;

	i = 0;
	while (cmds)
	{
		if (!cmds->argv || !cmds->argv[0] || !cmds->is_valid)
		{
			tmp = cmds;
			cmds = cmds->next;
			free_one_cmd(tmp);
			continue ;
		}
		if (run_single_cmd(&cmds, sh, in_fd, &pids[i]) == FAILURE)
		{
			if (cmds && cmds->redirect_failed)
			{
				pids[i] = -1;
				i++;
				continue ;
			}
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

static int	run_single_builtin_in_parent(t_cmd *cmd, t_msh *sh)
{
	int	saved_stdin;
	int	saved_stdout;
	int	dummy_pipe[2];

	if (!cmd->argv || !cmd->argv[0])
	{
		if (setup_redirections(cmd, sh, STDIN_FILENO, NULL) == FAILURE)
			return (g_exit);
		return (SUCCESS);
	}
	if (cmd->argv[0][0] == '\0' || !cmd->is_valid)
	{
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		g_exit = 127;
		return (127);
	}
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	dummy_pipe[0] = -1;
	dummy_pipe[1] = -1;
	if (setup_redirections(cmd, sh, STDIN_FILENO, dummy_pipe) == FAILURE)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (g_exit);
	}
	g_exit = run_builtin(cmd, sh);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (g_exit);
}

static int	has_valid_cmd(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->argv && cmd->argv[0] && cmd->is_valid)
			return (SUCCESS);
		cmd = cmd->next;
	}
	return (FAILURE);
}

int	exec_pipeline(t_cmd *cmds, t_msh *sh, int in_fd)
{
	int		cmd_count;
	pid_t	*pids;
	int		status;

	pids = NULL;
	if (!cmds)
		return (FAILURE);
	if (!cmds->next && cmds->is_builtin)
	{
		status = run_single_builtin_in_parent(cmds, sh);
		free_cmd(cmds);
		return (status);
	}
	if (has_valid_cmd(cmds) == FAILURE)
	{
		free_cmd(cmds);
		return (SUCCESS);
	}
	cmd_count = count_commands(cmds);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		free_cmd(cmds);
		return (FAILURE);
	}
	if (execute_all(cmds, sh, &in_fd, pids) == FAILURE)
	{
		free(pids);
		free_cmd(cmds);
		return (FAILURE);
	}
	status = wait_all_children(pids, cmd_count);
	print_redirect_error(cmds);
	free(pids);
	free_cmd(cmds);
	cmds = NULL;
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	return (status);
}

int	exec_ast(t_token *tokens, t_msh *sh)
{
	t_cmd	*cmds;
	int		in_fd;
	int		status;

	sh->error_printed = false;
	cmds = cmd_from_tokens(tokens, sh);
	if (!cmds)
	{
		g_exit = 0;
		return (SUCCESS);
	}
	in_fd = 0;
	free_token_list(tokens);
	status = exec_pipeline(cmds, sh, in_fd);
	g_exit = status;
	return (status);
}
