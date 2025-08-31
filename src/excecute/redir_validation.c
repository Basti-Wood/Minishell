/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_validation.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	apply_append_redir(t_redir *redir)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
		return (1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	process_redir_with_validation(t_redir *redir)
{
	if (!redir || !redir->filename)
		return (1);
	if (validate_single_redir(redir) != 0)
		return (1);
	if (redir->type == REDIR_INPUT)
		return (apply_input_redir(redir));
	else if (redir->type == REDIR_OUTPUT)
		return (apply_output_redir(redir));
	else if (redir->type == REDIR_APPEND)
		return (apply_append_redir(redir));
	return (0);
}
