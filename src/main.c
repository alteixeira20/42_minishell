/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 02:04:50 by paalexan          #+#    #+#             */
/*   Updated: 2025/05/23 17:10:03 by paalexan         ###   ########.fr       */
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
	status = check_cmd_syntax(tokens, true);
	if (status == SYNTAX_INCOMPLETE)
	{
		free_token_list(tokens);
		free(line);
		return (ft_strdup(""));
	}	
	while (status != SYNTAX_OK)
	{
		if (tokens)
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
		status = check_cmd_syntax(tokens, true);
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
	if ((g_exit == 130 || g_exit == 1) && (!tokens || !tokens->value))
	{
		if (tokens)
			free_token_list(tokens);
		free(line);
		g_exit = 1;
		return (1);
	}
	if (check_cmd_syntax(tokens, true) != SYNTAX_OK)
	{
		if (tokens)
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
		set_interactive_signals();
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
	sh = get_shell();
	if (init_sh(sh, env) != SUCCESS)
		exit_error(INIT_ERR, errno);
	loop(sh);
	free_env_array(sh->env);
	free_minishell(sh);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
