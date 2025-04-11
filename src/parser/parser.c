/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:01:14 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/11 14:53:42 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token_type	get_token_type(const char *str)
{
	if (!str)
		return (TOKEN_WORD);
	if (ft_strncmp(str, "|", 2) == 0)
		return (TOKEN_PIPE);
	if (ft_strncmp(str, ">", 2) == 0)
		return (TOKEN_REDIRECT_OUT);
	if (ft_strncmp(str, "<", 2) == 0)
		return (TOKEN_REDIRECT_IN);
	if (ft_strncmp(str, ">>", 3) == 0)
		return (TOKEN_APPEND);
	if (ft_strncmp(str, "<<", 3) == 0)
		return (TOKEN_HEREDOC);
	return (TOKEN_WORD);
}

static int	fill_tokens(char **split, t_token **tokens, t_minishell *sh)
{
	int				i;
	t_token			*new;
	t_token_type	type;
	char			*expanded;

	i = 0;
	while (split[i])
	{
		type = get_token_type(split[i]);
		expanded = expand_token_value(split[i], sh);
		new = token_new(expanded, type);
		free(expanded);
		if (!new)
			return (FAILURE);
		token_add_back(tokens, new);
		i++;
	}
	return (SUCCESS);
}

t_token	*parse_input(const char *line, t_minishell *sh)
{
	char			**split;
	t_token			*tokens;

	tokens = NULL;
	if (!line)
		return (NULL);
	split = split_input(line);
	if (!split)
		return (NULL);
	if (fill_tokens(split, &tokens, sh) == FAILURE)
	{
		free_split(split);
		free_token_list(tokens);
		return (NULL);
	}
	free_split(split);
	return (tokens);
}
