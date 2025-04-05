/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:03:17 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/05 13:09:01 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cmd_env(t_cmd *cmd, t_minishell *sh)
{
	int	i;

	if (!sh || !sh->env)
		return (FAILURE);
	if (cmd->argc > 1)
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		return (FAILURE);
	}
	i = 0;
	while (sh->env[i])
	{
		if (ft_strchr(sh->env[i], '='))
		{
			ft_putstr_fd(sh->env[i], STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		i++;
	}
	return (SUCCESS);
}
