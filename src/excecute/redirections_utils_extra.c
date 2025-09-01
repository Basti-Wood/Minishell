/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils_extra.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	apply_input_redir_safe(t_redir *redir, int *saved_stdin)
{
	int	fd;

	if (!redir || !redir->filename)
		return (0);
	if (access(redir->filename, F_OK) == -1)
	{
		ft_fprintf_stderr("%s: No such file or directory\n", redir->filename);
		return (1);
	}
	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	if (*saved_stdin == -1)
		*saved_stdin = dup(STDIN_FILENO);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_output_redir_safe(t_redir *redir, int *saved_stdout)
{
	int	fd;
	int	flags;

	if (!redir || !redir->filename)
		return (1);
	if (redir->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	if (*saved_stdout == -1)
		*saved_stdout = dup(STDOUT_FILENO);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}
