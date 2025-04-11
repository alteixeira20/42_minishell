
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:16:54 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/10 21:47:47 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	quote_type(char arg)
{
	if (arg == '\'')
		return (1);
	else if (arg == '\"')
		return (2);
	else
		return (0);
}
