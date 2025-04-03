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

void    shlvl(t_minishell *sh)
{
    char    *shlvl;
    char    *new;
    int     i;

    shlvl = extract_var("SHLVL", sh->envp);
    if(shlvl)
    {
        i = (ft_atoi(shlvl) + 1):
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