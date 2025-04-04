/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:22:37 by paalexan          #+#    #+#             */
/*   Updated: 2025/04/04 03:36:04 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_arg(t_cmd *cmd, char *value)
{
	int		i;
	char	**new_argv;

	i = 0;
	while (cmd->argv && cmd->argv[i])
		i++;
	new_argv = malloc(sizeof(char *) * (i + 2));
	if (!new_argv)
		return (FAILURE);
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i++] = ft_strdup(value);
	new_argv[i] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
	cmd->argc++;
	return (SUCCESS);
}

t_cmd	*cmd_new(void)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->argc = 0;
	cmd->argv = NULL;
	cmd->is_builtin = false;
	cmd->next = NULL;
	return (cmd);
}

static char	*prompt_user(t_minishell *sh)
{
	char	*result;
	char	*temp;

	result = ft_strjoin(sh->user, "@");
	temp = ft_strjoin(result, sh->hostname);
	free(result);
	temp = ft_strtrim(temp, "\n");
	result = ft_strjoin(temp, ":");
	free(temp);
	return (result);
}

char	*build_prompt(t_minishell *sh)
{
	char	*result;
	char	*temp;
	char	*cwd;
	char	*pwd;
	char	*home;

	temp = prompt_user(sh);
	cwd = extract_var("PWD", sh->env);
	if (cwd == NULL)
		cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		home = extract_var("HOME", sh->env);
		if (home == NULL)
			home = ft_strdup(sh->home);
		pwd = build_cwd(cwd, home);
		if (pwd)
			temp = ft_strjoin(temp, pwd);
		free(cwd);
		free(home);
	}
	result = ft_strjoin(temp, "$ ");
	return (free(temp), result);
}

char	*build_cwd(char *cwd, char *home)
{
	char	*result;
	int		i;
	int		cwd_len;
	int		home_len;

	cwd_len = ft_strlen(cwd);
	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) != 0)
		home_len = 0;
	result = ft_calloc((cwd_len - home_len + (home_len > 0) + 1), sizeof(char));
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
