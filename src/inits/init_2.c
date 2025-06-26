/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopedro- <jopedro-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:48:24 by jopedro-          #+#    #+#             */
/*   Updated: 2025/06/26 16:13:22 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_msh	*get_shell(void)
{
	static t_msh	sh;

	return (&sh);
}

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

void	get_shlvl(t_msh *sh)
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
