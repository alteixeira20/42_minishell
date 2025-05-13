/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:12:52 by paalexan          #+#    #+#             */
/*   Updated: 2025/05/13 17:31:28 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ctrl_c_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_redisplay();
	write(STDOUT_FILENO, "\n", 1);
	g_exit = 1;
}

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_exit = 130;
	close(STDIN_FILENO);
}

void	set_interactive_signals(void)
{
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_DFL);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_DFL);
}

void	reset_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}
