/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	save_io_fds(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin == -1)
		return (1);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdout == -1)
	{
		close(*saved_stdin);
		return (1);
	}
	return (0);
}

void	restore_io_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

int	handle_all_redirections(t_cmd *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	result;

	if (!cmd)
		return (0);
	if (save_io_fds(&saved_stdin, &saved_stdout))
		return (1);
	result = handle_redirections(cmd);
	if (result)
		restore_io_fds(saved_stdin, saved_stdout);
	return (result);
}

int	setup_pipe_redirections(t_cmd *cmd, int *pipe_fds)
{
	int	result;

	result = handle_all_redirections(cmd);
	if (result)
		return (result);
	if (pipe_fds && pipe_fds[1] != -1)
	{
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
			return (1);
		close(pipe_fds[1]);
	}
	return (0);
}

void	cleanup_pipe_redirections(int *pipe_fds)
{
	if (pipe_fds)
	{
		if (pipe_fds[0] != -1)
			close(pipe_fds[0]);
		if (pipe_fds[1] != -1)
			close(pipe_fds[1]);
	}
}
