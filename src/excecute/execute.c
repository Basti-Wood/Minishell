/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:15:25 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 17:15:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_redir(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_INPUT)
	{
		fd = open(redir->filename, O_RDONLY);
		if (fd < 0)
		{
			perror(redir->filename);
			return (-1);
		}
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			return (-1);
		}
		close(fd);
	}
	else if (redir->type == REDIR_OUTPUT)
	{
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror(redir->filename);
			return (-1);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			return (-1);
		}
		close(fd);
	}
	else if (redir->type == REDIR_APPEND)
	{
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror(redir->filename);
			return (-1);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			return (-1);
		}
		close(fd);
	}
	return (0);
}

static int	apply_redirections(t_cmd *cmd)
{
	t_redir	*current;

	current = cmd->redirs;
	while (current)
	{
		if (handle_redir(current) < 0)
			return (-1);
		current = current->next;
	}
	if (cmd->heredoc > 0)
	{
		if (dup2(cmd->heredoc, STDIN_FILENO) < 0)
		{
			close(cmd->heredoc);
			return (-1);
		}
		close(cmd->heredoc);
	}
	return (0);
}

int	execute_command(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(cmd) < 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	if (is_builtin(cmd->argv[0]))
		status = execute_builtin(cmd, shell);
	else
		status = execute_external(cmd, shell);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (status);
}
