/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 00:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 00:00:00 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_input_redirection(t_redir *redir)
{
	int	fd;

	if (validate_input_redir(redir) == -1)
		return (-1);
	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_output_redirection(t_cmd *cmd)
{
	t_redir	*current;
	int		fd;
	int		flags;

	current = cmd->redirs;
	while (current)
	{
		if (current->type == REDIR_OUTPUT || current->type == REDIR_APPEND)
		{
			if (validate_output_redir(current) == -1)
				return (-1);
			if (current->type == REDIR_APPEND)
				flags = O_WRONLY | O_CREAT | O_APPEND;
			else
				flags = O_WRONLY | O_CREAT | O_TRUNC;
			fd = open(current->filename, flags, 0644);
			if (fd == -1)
			{
				perror(current->filename);
				return (-1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				close(fd);
				return (-1);
			}
			close(fd);
		}
		current = current->next;
	}
	return (0);
}

static int	handle_heredoc_redirection(t_cmd *cmd)
{
	if (cmd->heredoc <= 0)
		return (0);
	if (dup2(cmd->heredoc, STDIN_FILENO) == -1)
	{
		perror("dup2");
		return (-1);
	}
	close(cmd->heredoc);
	cmd->heredoc = -1;
	return (0);
}

static int	apply_input_redirections(t_cmd *cmd)
{
	t_redir	*current;

	current = cmd->redirs;
	while (current)
	{
		if (current->type == REDIR_INPUT)
		{
			if (handle_input_redirection(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

int	apply_redirections(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (handle_heredoc_redirection(cmd) == -1)
		return (-1);
	if (apply_input_redirections(cmd) == -1)
		return (-1);
	if (handle_output_redirection(cmd) == -1)
		return (-1);
	
	return (0);
}