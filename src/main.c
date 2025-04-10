/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:04:50 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/11 00:20:29 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	g_exit = 0;

static void	ctrl_c_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit = 1;
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
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	loop(sh);
	free_env_array(sh->env);
	free_minishell(sh);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
