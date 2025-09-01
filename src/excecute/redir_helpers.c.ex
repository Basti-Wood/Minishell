/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 16:00:00 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_input_redir(t_redir *redir, int *fd)
{
	if (!redir || !redir->filename)
		return (0);
	if (*fd != -1)
		close(*fd);
	if (access(redir->filename, F_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: No such file or directory\n",
			redir->filename);
		return (-1);
	}
	if (access(redir->filename, R_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n",
			redir->filename);
		return (-1);
	}
	*fd = open(redir->filename, O_RDONLY);
	if (*fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (0);
}

int	apply_input_fd(int fd)
{
	if (fd == -1)
		return (0);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	process_output_redir(t_redir *redir, int *fd)
{
	int	flags;

	if (!redir || !redir->filename)
		return (0);
	if (*fd != -1)
		close(*fd);
	if (redir->type == REDIR_OUTPUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (redir->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		return (0);
	*fd = open(redir->filename, flags, 0644);
	if (*fd == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n",
			redir->filename);
		return (-1);
	}
	return (0);
}

int	validate_redir_list(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (!current->filename)
			return (-1);
		if (current->type == REDIR_INPUT)
		{
			if (access(current->filename, F_OK) == -1)
			{
				ft_fprintf_stderr("minishell: %s: No such file or directory\n",
					current->filename);
				return (-1);
			}
		}
		current = current->next;
	}
	return (0);
}
