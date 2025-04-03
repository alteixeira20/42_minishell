/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:39:09 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/03 03:31:00 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_builtin_cmd(const char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (true);
	return (false);
}

static int	count_tokens(t_token *tokens)
{
	int		counter;
	t_token	*tmp;

	tmp = tokens;
	counter = 0;
	while (tmp)
	{
		counter++;
		tmp = tmp->next;
	}
	return (counter);
}

static t_cmd	*fill_cmd(t_cmd *cmd, t_token *tokens, int counter)
{
	t_token	*tmp;
	int		i;

	tmp = tokens;
	cmd->argv = malloc(sizeof(char *) * (counter + 1));
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	while (tmp)
	{
		cmd->argv[i] = ft_strdup(tmp->value);
		tmp = tmp->next;
		i++;
	}
	cmd->argv[i] = NULL;
	cmd->argc = counter;
	cmd->is_builtin = is_builtin_cmd(cmd->argv[0]);
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->next = NULL;
	return (cmd);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		free(cmd->argv[i]);
		i++;
	}
	free(cmd->argv);
	free(cmd);
}

t_cmd	*cmd_from_tokens(t_token *tokens)
{
	t_cmd	*cmd;
	int		counter;

	counter = count_tokens(tokens);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	return (fill_cmd(cmd, tokens, counter));
}
