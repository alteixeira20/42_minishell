/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:04:50 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/03 03:32:41 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_putstr_fd("Token: [", STDOUT_FILENO);
		ft_putstr_fd(tokens->value, STDOUT_FILENO);
		ft_putstr_fd("]\n", STDOUT_FILENO);
		tokens = tokens->next;
	}
}

int	main(void)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmd;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		if (ft_strncmp(line, "exit", 5) == 0 && ft_strlen(line) == 4)
		{
			free(line);
			break ;
		}
		tokens = parse_input(line);
		if (tokens)
		{
			cmd = cmd_from_tokens(tokens);
			if (cmd)
			{
				if (cmd->is_builtin && ft_strncmp(cmd->argv[0], "echo", 5) == 0)
					cmd_echo(cmd);
				else
				{
					ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
					ft_putstr_fd(": command not found\n", STDERR_FILENO);
				}
				free_cmd(cmd);
			}
		}
		free_token_list(tokens);
		free(line);
	}
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	return (0);
}
