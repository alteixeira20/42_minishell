/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:01:14 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/03 03:31:49 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token_type	get_token_type(const char *str)
{
	if (ft_strncmp(str, "|", 2) == 0)
		return (TOKEN_PIPE);
	if (ft_strncmp(str, "<", 2) == 0)
		return (TOKEN_REDIRECT_IN);
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

t_token	*parse_input(const char *line)
{
	char	**args;
	t_token	*tokens;
	int		i;

	if (!line)
		return (NULL);
	args = split_input(line);
	if (!args)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (args[i])
	{
		token_add_back(&tokens, token_new(args[i], get_token_type(args[i])));
		i++;
	}
	free_args(args);
	return (tokens);
}
