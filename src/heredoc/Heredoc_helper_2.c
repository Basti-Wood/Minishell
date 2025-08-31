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

int	write_heredoc_line(char *content, int pipe_fd)
{
	write(pipe_fd, content, ft_strlen(content));
	write(pipe_fd, "\n", 1);
	return (0);
}

int	process_heredoc_line(char *line, t_heredoc_data *data)
{
	char	*expanded;

	if (ft_strcmp(line, data->delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (data->expand)
	{
		expanded = expand_variables(line, data->shell);
		free(line);
		if (!expanded)
			return (-1);
		write_heredoc_line(expanded, data->pipe_fd);
		free(expanded);
	}
	else
	{
		write_heredoc_line(line, data->pipe_fd);
		free(line);
	}
	return (0);
}

void	heredoc_child_main(t_heredoc_data *data)
{
	char	*line;
	int		result;

	setup_heredoc_signals_internal();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: doc delimited by end-of-file\n",
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
