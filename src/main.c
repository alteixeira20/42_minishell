/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:04:50 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/27 19:05:23 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	g_exit = 0;

static char	*read_continuation_loop(char *line, t_msh *sh)
{
	char		*next;
	char		*joined;
	t_token		*tokens;
	t_syntax	status;

	tokens = parse_input(line, sh);
	status = check_cmd_syntax(tokens, false);
	while (status == SYNTAX_INCOMPLETE)
	{
		free_token_list(tokens);
		next = readline("> ");
		if (!next)
			break ;
		joined = ft_strjoin(line, next);
		free(line);
		free(next);
		if (!joined)
			break ;
		line = joined;
		tokens = parse_input(line, sh);
		status = check_cmd_syntax(tokens, false);
	}
	free_token_list(tokens);
	return (line);
}

static char	*read_multiline_input(t_msh *sh)
{
	char	*line;
	char	*prompt;

	prompt = build_prompt(sh);
	line = readline(prompt);
	free(prompt);
	if (!line)
		return (NULL);
	line = read_continuation_loop(line, sh);
	return (line);
}

static int	handle_input_line(t_msh *sh, char *line)
{
	t_token	*tokens;

	if (*line)
		add_history(line);
	tokens = parse_input(line, sh);
	if (g_exit == 130)
	{
		free_token_list(tokens);
		free(line);
		g_exit = 1;
		return (1);
	}
	if (check_cmd_syntax(tokens, true) == SYNTAX_ERROR)
	{
		free_token_list(tokens);
		free(line);
		return (1);
	}
	g_exit = exec_ast(tokens, sh);
	free(line);
	return (1);
}

int	loop(t_msh *sh)
{
	char	*line;

	while (1)
	{
		line = read_multiline_input(sh);
		if (!line)
			break ;
		if (!handle_input_line(sh, line))
			break ;
	}
	return (1);
}

int	main(int ac, char **av, char **env)
{
	t_msh	*sh;

	(void)av;
	(void)ac;
	sh = ft_calloc(1, sizeof(t_msh));
	if (sh == NULL)
		return (exit_error(MALLOC_ERR, errno), EXIT_FAILURE);
	if (init_sh(sh, env) != SUCCESS)
		exit_error(INIT_ERR, errno);
	set_interactive_signals();
	loop(sh);
	free_env_array(sh->env);
	free_minishell(sh);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
