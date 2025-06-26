/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:59:33 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 17:05:49 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clean_all(t_msh *sh, t_cmd *cmd)
{
	clean_fds();
	free_cmd(cmd);
	free_heredoc_tmpfiles(sh->heredoc_tmpfiles);
	free_env_array(sh->env);
	free_minishell(sh);
}
