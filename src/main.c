/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:04:50 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/10 22:58:48 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	g_exit = 0;

static bool	needs_pipe_continuation(const char *line)
{
	int	i;

	i = ft_strlen(line);
	while (i > 0 && ft_isspace(line[i - 1]))
		i--;
	if (i > 0 && line[i - 1] == '|')
		return (true);
	return (false);
}

static char	*read_multiline_input(t_minishell *sh)
{
	char	*line;
	char	*next;
	char	*joined;
	char	*prompt;

	prompt = build_prompt(sh);
	line = readline(prompt);
	free(prompt);
	while (line && needs_pipe_continuation(line))
	{
		next = readline("> ");
		joined = ft_strjoin(line, next);
		free(line);
		free(next);
		line = joined;
	}
	return (line);
}

int	loop(t_minishell *sh)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		line = read_multiline_input(sh);
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
	free_env_array(sh->env);
	free_minishell(sh);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
