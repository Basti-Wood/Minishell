/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:05:32 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:05:33 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

int	prepare_heredoc_delimiter(char *delimiter, char **tmp_delim,
		int *expand)
{
	*expand = 1;
	*tmp_delim = NULL;
	if (delimiter[0] == '\'' || delimiter[0] == '"')
	{
		*expand = 0;
		*tmp_delim = ft_strtrim(delimiter, "\"'");
		if (!*tmp_delim)
		{
			perror("malloc");
			return (-1);
		}
	}
	return (0);
}

int	setup_heredoc_pipe(int pipefd[2], char *tmp_delim)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		if (tmp_delim)
			free(tmp_delim);
		return (-1);
	}
	return (0);
}

pid_t	fork_heredoc_process(int pipefd[2], char *tmp_delim)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		if (tmp_delim)
			free(tmp_delim);
		return (-1);
	}
	return (pid);
}
