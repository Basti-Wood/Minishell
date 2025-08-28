/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 15:37:26 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 15:37:30 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_input_redir(t_redir *redir, int *fd)
{
	if (!redir || !redir->filename)
		return (-1);
	if (*fd != -1)
		close(*fd);
	*fd = open(redir->filename, O_RDONLY);
	if (*fd == -1)
	{
		perror(redir->filename);
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
		perror("dup2 input");
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
		return (-1);
	if (*fd != -1)
		close(*fd);
	if (redir->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	*fd = open(redir->filename, flags, 0644);
	if (*fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	return (0);
}
