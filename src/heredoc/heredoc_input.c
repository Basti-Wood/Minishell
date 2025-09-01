/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 00:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/09/01 00:00:00 by seftekha         ###   ########.fr       */
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

char	*read_heredoc_line(void)
{
	if (isatty(STDIN_FILENO))
		return (readline("> "));
	else
		return (read_line_from_stdin());
}

static int	process_and_write_line(char *line, t_shell *shell,
		int pipe_fd, int expand)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_variables(line, shell);
		free(line);
		if (!expanded)
			return (-1);
		write(pipe_fd, expanded, ft_strlen(expanded));
		write(pipe_fd, "\n", 1);
		free(expanded);
	}
	else
	{
		write(pipe_fd, line, ft_strlen(line));
		write(pipe_fd, "\n", 1);
		free(line);
	}
	return (0);
}

static int	process_heredoc_loop(char *delimiter, t_shell *shell,
		int pipe_fd, int expand)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line();
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				ft_putstr_fd("minishell: doc delimited by end-of-file\n",
					STDERR_FILENO);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (process_and_write_line(line, shell, pipe_fd, expand) == -1)
			return (-1);
	}
	return (0);
}

int	process_heredoc_content(char *delimiter, t_shell *shell,
		int pipe_fd, int expand)
{
	int		original_stdin;
	int		result;

	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
		return (-1);
	result = process_heredoc_loop(delimiter, shell, pipe_fd, expand);
	close(original_stdin);
	if (result == -1)
		return (-1);
	return (0);
}
