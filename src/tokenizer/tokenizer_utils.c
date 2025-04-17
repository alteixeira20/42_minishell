/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 01:07:31 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/17 16:40:19 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*unescape_token(const char *str)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
			i++;
		res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

int	is_special_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

char	*extract_special_char(const char *str, int *i)
{
	int	start;

	start = *i;
	if ((str[*i] == '>' || str[*i] == '<') && str[*i] == str[*i + 1])
		(*i) += 2;
	else
		(*i) += 1;
	return (ft_substr(str, start, *i - start));
}

static int	handle_redirect_token(t_token *tokens, t_cmd *cmd, t_msh *sh)
{
	if (tokens->type == TOKEN_REDIRECT_IN
		|| tokens->type == TOKEN_REDIRECT_OUT
		|| tokens->type == TOKEN_APPEND)
	{
		if (process_redirect(cmd, tokens, sh) == -1)
			return (-1);
		tokens = tokens->next;
	}
	else if (tokens->type == TOKEN_HEREDOC)
	{
		if (handle_redirect_heredoc(cmd, tokens->next, sh) == -1)
			return (-1);
		tokens = tokens->next;
	}
	return (0);
}

int	process_token(t_token *tokens, t_cmd **current, t_msh *sh)
{
	int	status;

	if (!tokens || !current || !*current || !sh)
		return (-1);
	if (tokens->type == TOKEN_WORD)
	{
		printf("[DEBUG] = tokens value [%s]\n", tokens->value);
		if (add_arg(*current, tokens->value) == FAILURE)
			return (-1);
	}
	else if (tokens->type == TOKEN_PIPE)
	{
		(*current)->next = cmd_new();
		if (!(*current)->next)
			return (-1);
		*current = (*current)->next;
	}
	else
	{
		status = handle_redirect_token(tokens, *current, sh);
		if (status == -1)
			(*current)->input_fd = -1;
	}
	return (0);}
