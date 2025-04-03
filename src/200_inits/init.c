/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_input_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:28:43 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/03 03:31:24 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char    *get_hostname(void)
{
    int     fd;
    char    *ret;
    char    *temp;

    fd = open("/etc/hostname", O_RDONLY);
    if (fd != -1)
    {
        ret = get_next_line(fd);
        temp = get_next_line(-1);
        if (ret == NULL || ret[0] == '\0')
            return (close(fd), ft_strdup("1337"));
        ret = hostname_norm(ret);
    }
    else
        return (close(fd), ft_strdup("1337"));
}

void    shlvl(t_minishell *sh)
{
    char    *shlvl;
    char    *new;
    int     i;

    i = 0;
    shlvl = extract_var("SHLVL", sh->env);
    if(shlvl)
    {
        i = (ft_atoi(shlvl) + 1);
        new = ft_itoa(i);
        if (new ==NULL)
            //error
        set_var("SHLVL", new, &sh->env);
        free(new);
        free(shlvl);
    }
    else
        set_var("SHLVL", "1", &sh->env);
}

int init(t_minishell *sh, char **env)
{
    sh->env = init_env(env);
    shlvl(sh);
    if (sh->env == NULL)
        return (exit_error(ENV_INIT_ERR, errno), FAILURE);
    sh->home = extract_var ("HOME", sh->env);
    sh->user = extract_var("USER", sh->env);
    sh->hostname = get_hostname();
    set_var("OLDPWD", NULL, &sh->env);
    //termios
    sh->pipe_cnt = 0;
    sh->hd_cnt = 0;
    sh->cmds = NULL;
    return(SUCCESS);
}