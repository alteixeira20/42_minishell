/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 04:04:54 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/18 00:40:09 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	surpress_par_sig(struct sigaction *o_int, struct sigaction *o_quit)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, o_int);
	sigaction(SIGQUIT, &sa, o_quit);
}

void	restore_par_sig(struct sigaction *o_int, struct sigaction *o_quit)
{
	sigaction(SIGINT, o_int, NULL);
	sigaction(SIGQUIT, o_quit, NULL);
}
