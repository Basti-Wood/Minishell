/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	apply_output_redirection(t_redir *redir, int flags)
{
	int	fd;

	if (!redir || !redir->filename)
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
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_input_redir_exec(t_redir *redir)
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
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	handle_single_redirection(t_redir *redir)
{
	if (!redir)
		return (0);
	if (redir->type == REDIR_INPUT)
		return (handle_input_redir_exec(redir));
	else
		return (apply_output_redirection(redir, 0));
}

int	process_single_redir(t_redir *redir, int *saved_stdin, int *saved_stdout)
{
	if (!redir)
		return (0);
	if (redir->type == REDIR_HEREDOC)
		return (0);
	if (redir->type == REDIR_INPUT)
		return (apply_input_redir_safe(redir, saved_stdin));
	if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
		return (apply_output_redir_safe(redir, saved_stdout));
	return (0);
}
