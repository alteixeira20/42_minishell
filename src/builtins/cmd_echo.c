/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:16:54 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/12 18:01:16 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static char	*strip_outer_quotes(const char *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len >= 2 && ((s[0] == '\'' && s[len - 1] == '\'')
			|| (s[0] == '"' && s[len - 1] == '"')))
		return (ft_substr(s, 1, len - 2));
	return (ft_strdup(s));
}

static void	print_echo_args(t_cmd *cmd, int i)
{
	char	*stripped;

	while (cmd->argv[i])
	{
		stripped = strip_outer_quotes(cmd->argv[i]);
		if (stripped)
		{
			ft_putstr_fd(stripped, cmd->output_fd);
			free(stripped);
		}
		if (cmd->argv[i + 1])
			ft_putchar_fd(' ', cmd->output_fd);
		i++;
	}
}

int	quote_type(char arg)
{
	if (arg == '\'')
		return (1);
	else if (arg == '\"')
		return (2);
	else
		return (0);
}

int	cmd_echo(t_cmd *cmd)
{
	int		i;
	bool	new_line;

	i = 1;
	new_line = true;
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	while (cmd->argv[i] && is_valid_n_flag(cmd->argv[i]))
	{
		new_line = false;
		i++;
	}
	print_echo_args(cmd, i);
	if (new_line)
		ft_putchar_fd('\n', cmd->output_fd);
	return (0);
}
