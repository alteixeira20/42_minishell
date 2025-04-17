/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:28:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/16 17:30:46 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_hostname(void)
{
	int		fd;
	char	*ret;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd != -1)
	{
		ret = get_next_line(fd);
		get_next_line(-1);
		if (ret == NULL || ret[0] == '\0')
			return (close(fd), ft_strdup("1337"));
		return (close(fd), ret);
	}
	else
		return (close(fd), ft_strdup("1337"));
}

void	shlvl(t_msh *sh)
{
	char	*shlvl;
	char	*new;
	int		i;

	if (!sh || !sh->env)
		return ;
	i = 0;
	shlvl = extract_var("SHLVL", sh->env);
	if (shlvl)
	{
		i = (ft_atoi(shlvl) + 1);
		new = ft_itoa(i);
		if (new == NULL)
			return ;
		set_var("SHLVL", new, &sh->env);
		free(new);
		free(shlvl);
	}
	else
		set_var("SHLVL", "1", &sh->env);
}

int	init(t_msh *sh, char **env)
{
	char	*pwd;

	sh->env = init_env(env);
	shlvl(sh);
	if (sh->env == NULL)
		return (exit_error(ENV_INIT_ERR, errno), FAILURE);
	sh->home = extract_var ("HOME", sh->env);
	sh->user = extract_var("USER", sh->env);
	sh->hostname = get_hostname();
	pwd = extract_var("PWD", sh->env);
	if (pwd)
	{
		set_var("OLDPWD", pwd, &sh->env);
		free(pwd);
	}
	sh->cmds = NULL;
	sh->cmd_cnt = 0;
	sh->pipes = NULL;
	sh->pipe_cnt = 0;
	sh->hd_cnt = 0;
	sh->path = NULL;
	sh->exit_status = 0;
	sh->heredoc_interrupted = false;
	sh->had_syntax_error = false;
	return (SUCCESS);
}
