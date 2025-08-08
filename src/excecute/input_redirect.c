/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:52:30 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 16:52:59 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	process_input_list(t_redir *redir, int *fd)
{
	if (!redir)
		return (0);
	if (process_input_redir(redir, fd) == -1)
		return (-1);
	return (process_input_list(redir->next, fd));
}

int	handle_input_redirection(t_cmd *cmd)
{
	int	fd;

	fd = -1;
	if (cmd->infiles && process_input_list(cmd->infiles, &fd) == -1)
		return (-1);
	if (apply_input_fd(fd) == -1)
		return (-1);
	if (cmd->heredoc > 0)
	{
		if (dup2(cmd->heredoc, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			return (-1);
		}
		close(cmd->heredoc);
	}
	return (0);
}
