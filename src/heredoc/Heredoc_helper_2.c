/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:05:32 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:05:33 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*read_line_from_stdin(void)
{
	char	*line;
	char	*buffer;
	size_t	len;
	ssize_t	read_result;

	buffer = NULL;
	len = 0;
	read_result = getline(&buffer, &len, stdin);
	if (read_result == -1)
	{
		if (buffer)
			free(buffer);
		return (NULL);
	}
	if (buffer[read_result - 1] == '\n')
		buffer[read_result - 1] = '\0';
	line = ft_strdup(buffer);
	free(buffer);
	return (line);
}

static char	*get_heredoc_input(int is_interactive)
{
	if (is_interactive && isatty(STDIN_FILENO))
		return (readline("> "));
	else
		return (read_line_from_stdin());
}

static void	heredoc_process_loop(t_heredoc_data *data, int is_interactive)
{
	char	*line;
	int		result;

	while (1)
	{
		line = get_heredoc_input(is_interactive);
		if (!line)
		{
			if (is_interactive)
				ft_putstr_fd("minishell: doc delimited by end-of-file\n",
					STDERR_FILENO);
			break ;
		}
		result = process_heredoc_line(line, data);
		if (result == 1)
			break ;
		if (result == -1)
		{
			close(data->pipe_fd);
			exit(1);
		}
	}
}

void	heredoc_child_main(t_heredoc_data *data)
{
	int		is_interactive;

	setup_heredoc_signals_internal();
	is_interactive = isatty(STDIN_FILENO);
	heredoc_process_loop(data, is_interactive);
	close(data->pipe_fd);
	exit(0);
}

int	heredoc_parent_wait(pid_t pid, int pipefd[2], char *tmp_delim)
{
	int	status;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (tmp_delim)
		free(tmp_delim);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
		{
			close(pipefd[0]);
			return (-1);
		}
		return (pipefd[0]);
	}
	close(pipefd[0]);
	return (-1);
}
