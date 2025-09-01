/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	apply_input_redir(t_redir *redir)
{
	int	fd;

	if (!redir->filename)
		return (1);
	if (access(redir->filename, F_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: No such file or directory\n",
			redir->filename);
		return (1);
	}
	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n",
			redir->filename);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_output_redir(t_redir *redir)
{
	int	fd;
	int	flags;

	if (!redir->filename)
		return (1);
	if (redir->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n",
			redir->filename);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_single_redir(t_redir *redir)
{
	if (!redir)
		return (0);
	if (redir->type == REDIR_INPUT)
		return (apply_input_redir(redir));
	if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
		return (apply_output_redir(redir));
	if (redir->type == REDIR_HEREDOC)
		return (0);
	return (0);
}

int	apply_all_redirs(t_cmd *cmd)
{
	t_redir	*current;
	int		result;

	current = cmd->redirs;
	while (current)
	{
		result = apply_single_redir(current);
		if (result != 0)
			return (result);
		current = current->next;
	}
	return (0);
}

int	handle_redirections(t_cmd *cmd)
{
	int	result;

	if (!cmd)
		return (0);
	result = apply_all_redirs(cmd);
	if (result != 0)
		return (result);
	if (cmd->heredoc > 0)
	{
		if (dup2(cmd->heredoc, STDIN_FILENO) == -1)
		{
			close(cmd->heredoc);
			return (1);
		}
		close(cmd->heredoc);
	}
	return (0);
}
