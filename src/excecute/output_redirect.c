/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:53:39 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 16:53:55 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	process_output_list(t_redir *redir, int *fd)
{
	if (!redir)
		return (0);
	if (process_output_redir(redir, fd) == -1)
		return (-1);
	return (process_output_list(redir->next, fd));
}

int	handle_output_redirection(t_cmd *cmd)
{
	int	fd;

	fd = -1;
	if (!cmd->outfiles)
		return (0);
	if (process_output_list(cmd->outfiles, &fd) == -1)
		return (-1);
	if (fd == -1)
		return (0);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 output");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
