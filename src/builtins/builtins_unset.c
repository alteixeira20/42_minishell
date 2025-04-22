/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopedro- <jopedro-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:22:16 by jopedro-          #+#    #+#             */
/*   Updated: 2025/04/21 17:53:06 by jopedro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

int		cmd_unset(t_cmd *cmd, t_msh *sh)
{
	char	*arg;
	int		i;

	i = 0;
	if (!cmd->argv[1] || sh->env)
		return (FAILURE);
	if (!cmd->argv[1][0] == '-' && cmd->argv[1][1] != '\0')
		return (exit_error("flag error", 1);
	while (cmd->argv[++i])
	{
		arg = cmd->argv[i];
		if (ft_strchr(arg, '=') == NULL)
			if (var_index(arg, sh->env) >= 0)
				sh->env = del_var(sh->env, arg);
	}
	return (SUCCESS);
}
