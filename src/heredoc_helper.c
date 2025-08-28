/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:17:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 15:34:43 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	cleanup_heredoc_delimiter(char *processed_delim, char *original_delim)
{
	if (processed_delim != original_delim)
		free(processed_delim);
}

int	fork_heredoc_process(int *pipefd, char *delimiter, int expand,
		t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_child_process(pipefd[1], delimiter, expand, shell);
	}
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	return (handle_heredoc_status(pipefd[0], status));
}

int	handle_heredoc_status(int readfd, int status)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(readfd);
		return (-1);
	}
	if (!WIFEXITED(status))
	{
		close(readfd);
		return (-1);
	}
	return (readfd);
}

void	heredoc_child_process(int writefd, char *delimiter, int expand,
		t_shell *shell)
{
	struct sigaction	sa;
	char				*line;

	sa.sa_handler = heredoc_sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	while (1)
	{
		line = readline("> ");
		if (handle_heredoc_eof(line))
			break ;
		if (handle_heredoc_delimiter(line, delimiter, expand))
			break ;
		write_heredoc_line(writefd, line, expand, shell);
	}
	close(writefd);
	exit(0);
}
