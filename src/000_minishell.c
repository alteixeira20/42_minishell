/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   000_minishell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:04:50 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/03 21:35:07 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit = 0;

int	loop(t_minishell *sh)
{
	char	*line;
	t_token	*tokens;

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
			exec_tokens(tokens, sh);
		free_token_list(tokens);
		free(line);
	}
	return (1);
}

int	main(int ac, char **av, char **env)
{
	t_minishell	*sh;

	(void)av;
	(void)ac;
	sh = ft_calloc(1, sizeof(t_minishell));
	if (sh == NULL)
		return (exit_error(MALLOC_ERR, errno), EXIT_FAILURE);
	if (init(sh, env) != SUCCESS)
		exit_error(INIT_ERR, errno);
	loop(sh);
	free(sh);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
