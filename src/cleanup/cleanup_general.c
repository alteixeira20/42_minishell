/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_general.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:31:36 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/25 18:28:06 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	clean_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

const char	*token_type_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return "WORD";
	if (type == TOKEN_PIPE)
		return "PIPE";
	if (type == TOKEN_REDIRECT_IN)
		return "REDIRECT_IN";
	if (type == TOKEN_REDIRECT_OUT)
		return "REDIRECT_OUT";
	if (type == TOKEN_APPEND)
		return "APPEND";
	if (type == TOKEN_HEREDOC)
		return "HEREDOC";
	return "UNKNOWN";
}

void	print_token(const t_token *token, int i)
{
	printf("[%d] Type: %s | Value: '%s'%s\n",
		i,
		token_type_str(token->type),
		token->value ? token->value : "(null)",
		token->expanded_empty ? " | (was expanded to empty)" : "");
}

void	free_token_list(t_token *token)
{
	t_token	*next;

	if (!token)
		return ;
	while (token && token->prev)
		token = token->prev;
	while (token)
	{
		next = token->next;
		free(token->value);
		free(token);
		token = next;
	}
}

void	free_env_array(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		free(env[i++]);
	if (env)
		free(env);
}

void	free_minishell(t_msh *sh)
{
	if (sh->home)
		free(sh->home);
	if (sh->user)
		free(sh->user);
	if (sh->hostname)
		free(sh->hostname);
	if (sh->tokens)
		free_token_list(sh->tokens);
	rl_clear_history();
}

