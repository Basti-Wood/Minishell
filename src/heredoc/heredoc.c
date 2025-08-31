/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:05:32 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:05:33 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	prepare_heredoc_setup(char *delimiter, char **tmp_delim,
		int *expand, char **final_delim)
{
	if (prepare_heredoc_delimiter(delimiter, tmp_delim, expand) == -1)
		return (-1);
	*final_delim = delimiter;
	if (*tmp_delim)
		*final_delim = *tmp_delim;
	return (0);
}

static void	setup_heredoc_data(t_heredoc_data *data, int pipe_fd,
		char *delimiter, int expand)
{
	data->pipe_fd = pipe_fd;
	data->delimiter = delimiter;
	data->expand = expand;
}

static int	handle_heredoc_child(int pipefd[2], char *final_delim, int expand,
		t_shell *shell)
{
	t_heredoc_data	data;

	close(pipefd[0]);
	setup_heredoc_data(&data, pipefd[1], final_delim, expand);
	data.shell = shell;
	heredoc_child_main(&data);
	return (0);
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int		pipefd[2];
	char	*tmp_delim;
	int		expand;
	char	*final_delim;

	if (prepare_heredoc_setup(delimiter, &tmp_delim, &expand, &final_delim)
		== -1)
		return (-1);
	if (setup_heredoc_pipe(pipefd, tmp_delim) == -1)
		return (-1);
	pipefd[0] = fork_heredoc_process(pipefd, tmp_delim);
	if (pipefd[0] == -1)
		return (-1);
	if (pipefd[0] == 0)
		handle_heredoc_child(pipefd, final_delim, expand, shell);
	return (heredoc_parent_wait(pipefd[0], pipefd, tmp_delim));
}
