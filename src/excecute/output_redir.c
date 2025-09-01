/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:53:42 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 15:54:03 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	open_output_file(const char *filename, int flags)
{
	int	fd;

	if (validate_output_file(filename))
		return (-1);
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		ft_fprintf_stderr("minishell: %s: Failed to open file\n", filename);
		return (-1);
	}
	return (fd);
}

static int	get_output_flags(int redir_type)
{
	if (redir_type == REDIR_APPEND)
		return (O_WRONLY | O_CREAT | O_APPEND);
	else
		return (O_WRONLY | O_CREAT | O_TRUNC);
}

static int	setup_output_redirection(int fd)
{
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		close(fd);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		close(saved_stdout);
		return (1);
	}
	return (0);
}

int	handle_output_redirection(t_redir *redir)
{
	int	fd;
	int	flags;

	if (!redir || !redir->filename)
		return (0);
	flags = get_output_flags(redir->type);
	fd = open_output_file(redir->filename, flags);
	if (fd == -1)
		return (1);
	if (setup_output_redirection(fd) != 0)
		return (1);
	close(fd);
	return (0);
}

int	apply_output_redirections(t_cmd *cmd)
{
	t_redir	*current;
	int		result;

	if (!cmd)
		return (0);
	current = cmd->outfiles;
	while (current)
	{
		result = handle_output_redirection(current);
		if (result)
			return (result);
		current = current->next;
	}
	return (0);
}
