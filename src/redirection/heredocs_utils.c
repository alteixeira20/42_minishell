/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:12:05 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/12 19:36:28 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*heredoc_tmpname(int index, t_msh *sh)
{
	char			*base;
	char			*num;
	char			*res;
	t_hdoc_tmpfile	*new_node;

	base = ft_strdup("/tmp/.minishell_heredoc_");
	num = ft_itoa(index);
	if (!base || !num)
	{
		free(base);
		free(num);
		return (NULL);
	}
	res = ft_strjoin(base, num);
	free(base);
	free(num);
	new_node = malloc(sizeof(t_hdoc_tmpfile));
	if (!new_node)
		return (res);
	new_node->path = res;
	new_node->next = sh->heredoc_tmpfiles;
	sh->heredoc_tmpfiles = new_node;
	return (res);
}

static void	write_heredoc_content(const char *delim, int fd, t_msh *sh)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_exit != 130)
				print_heredoc_warning(delim);
			else
				ft_putstr_fd("\n", STDOUT_FILENO);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		line = handle_expansion(line, sh);
		ft_putendl_fd(line, fd);
		free(line);
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

void	print_tokens(t_token *tokens)
{
	int i = 0;

	printf("Tokens:\n");
	while (tokens)
	{
		printf("[%d] Type: %s | Value: '%s'%s\n",
			i++,
			token_type_str(tokens->type),
			tokens->value ? tokens->value : "(null)",
			tokens->expanded_empty ? " | (was expanded to empty)" : "");
		tokens = tokens->next;
	}
}

static int	run_heredoc_child(const char *delim, int fd, t_msh *sh, t_token *tokens)
{
	setup_heredoc_signals();
	write_heredoc_content(delim, fd, sh);
	clean_fds();
	free_token_list(tokens);
	free_cmd(sh->cmds);
	free_env_array(sh->env);
	free_minishell(sh);
	exit(0);
}

static char	*handle_heredoc_status(int status, char *filename)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(filename);
		free(filename);
		g_exit = 130;
		return (NULL);
	}
	return (filename);
}

char	*write_heredoc_to_tmp(const char *delim, int index, t_msh *sh, t_token *tokens)
{
	char			*filename;
	int				fd;
	pid_t			pid;
	int				status;

	filename = heredoc_tmpname(index, sh);
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(filename);
		g_exit = 1;
		return (NULL);
	}
	pid = fork();
	if (pid == 0)
		run_heredoc_child(delim, fd, sh, tokens);
	close(fd);
	waitpid(pid, &status, 0);
	return (handle_heredoc_status(status, filename));
}
