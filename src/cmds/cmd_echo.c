/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:16:54 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/03 03:47:11 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_valid_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

static void	copy_unquoted(char *res, const char *arg)
{
	int	i;
	int	j;
	int	quote;

	i = 0;
	j = 0;
	while (arg[i])
	{
		if (arg[i] == '\'' || arg[i] == '"')
		{
			quote = arg[i];
			i++;
			while (arg[i] && arg[i] != quote)
				res[j++] = arg[i++];
			if (arg[i] == quote)
				i++;
		}
		else
			res[j++] = arg[i++];
	}
	res[j] = '\0';
}

static char	*strip_quotes(const char *arg)
{
	char	*res;

	if (!arg)
		return (NULL);
	res = malloc(ft_strlen(arg) + 1);
	if (!res)
		return (NULL);
	copy_unquoted(res, arg);
	return (res);
}

int	cmd_echo(t_cmd *cmd)
{
	int		i;
	bool	new_line;
	char	*stripped;

	i = 1;
	new_line = true;
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	while (cmd->argv[i] && is_valid_n_flag(cmd->argv[i]))
	{
		new_line = false;
		i++;
	}
	while (cmd->argv[i])
	{
		stripped = strip_quotes(cmd->argv[i]);
		if (stripped)
		{
			ft_putstr_fd(stripped, STDOUT_FILENO);
			free(stripped);
		}
		if (cmd->argv[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (new_line)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
