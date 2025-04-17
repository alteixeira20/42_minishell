/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:48:01 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/16 19:58:01 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*prompt_user(t_msh *sh)
{
	char	*step1;
	char	*step2;
	char	*trimmed;
	char	*final;

	step1 = ft_strjoin(sh->user, "@");
	if (!step1)
		return (NULL);
	step2 = ft_strjoin(step1, sh->hostname);
	free(step1);
	if (!step2)
		return (NULL);
	trimmed = ft_strtrim(step2, "\n");
	free(step2);
	if (!trimmed)
		return (NULL);
	final = ft_strjoin(trimmed, ":");
	free(trimmed);
	return (final);
}

static char	*build_cwd(char *cwd, char *home)
{
	char	*result;
	int		i;
	int		cwd_len;
	int		home_len;
	int		size;

	cwd_len = ft_strlen(cwd);
	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) != 0)
		home_len = 0;
	size = cwd_len - home_len + (home_len > 0) + 1;
	result = ft_calloc(size, sizeof(char));
	i = 0;
	if (home_len > 0)
		result[0] = '~';
	while (i < (cwd_len - home_len))
	{
		result[i + (home_len > 0)] = cwd[i + home_len];
		i++;
	}
	result[i + (home_len > 0)] = '\0';
	return (result);
}

static char	*append_pwd(char *prompt, char *cwd, char *home)
{
	char	*cwd_display;
	char	*joined;

	cwd_display = build_cwd(cwd, home);
	if (!cwd_display)
		return (prompt);
	joined = ft_strjoin(prompt, cwd_display);
	free(prompt);
	free(cwd_display);
	return (joined);
}

bool	needs_pipe_continuation(const char *line)
{
	int	i;

	i = ft_strlen(line);
	while (i > 0 && ft_isspace(line[i - 1]))
		i--;
	if (i > 0 && line[i - 1] == '|')
		return (true);
	return (false);
}

char	*build_prompt(t_msh *sh)
{
	char	*result;
	char	*prompt;
	char	*cwd;
	char	*home;

	prompt = prompt_user(sh);
	cwd = extract_var("PWD", sh->env);
	if (!cwd)
		cwd = getcwd(NULL, 0);
	if (cwd)
	{
		home = extract_var("HOME", sh->env);
		if (!home)
			home = ft_strdup(sh->home);
		prompt = append_pwd(prompt, cwd, home);
		free(home);
		free(cwd);
	}
	result = ft_strjoin(prompt, "$ ");
	free(prompt);
	return (result);
}
