/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:01:14 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/26 13:54:52 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token_type	get_token_type(const char *str)
{
	if (!str)
		return (TOKEN_WORD);
	if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(str, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	if (ft_strcmp(str, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	return (TOKEN_WORD);
}

static int	fill_tokens(char **split, t_token **tokens, t_msh *sh)
{
	int				i;
	t_token			*new;
	t_token_type	type;

	i = 0;
	while (split[i])
	{
		type = get_token_type(split[i]);
		new = token_new(split[i], type, sh);
		if (!new)
		{
			free_split(split);
			if (*tokens)
				free_token_list(*tokens);
			return (FAILURE);
		}
		if (new->expanded_empty && new->value[0] == '\0')
		{
			free(new->value);
			free(new);
		}
		else
			token_add_back(tokens, new);
		i++;
	}
	return (SUCCESS);
}

static char	*strip_unquoted_comment(char *line)
{
	int		i;
	char	quote;
	char	*new_line;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '"') && !quote)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		else if (line[i] == '#' && quote == 0)
			break ;
		i++;
	}
	new_line = ft_substr(line, 0, i);
	return (new_line);
}

t_token	*parse_input(const char *line, t_msh *sh)
{
	char	*clean_line;
	char	**split;
	t_token	*tokens;

	tokens = NULL;
	if (!line)
		return (NULL);
	clean_line = strip_unquoted_comment((char *)line);
	if (!clean_line)
		return (NULL);
	split = split_input(clean_line);
	free(clean_line);
	if (!split)
	{
		ft_putendl_fd("syntax error near unexpected token `newline'", STDERR_FILENO);
		g_exit = 2;
		return (NULL);
	}
	if (fill_tokens(split, &tokens, sh) == FAILURE)
	{
		free_split(split);
		free_token_list(tokens);
		return (NULL);
	}
	free_split(split);
	return (tokens);
}
