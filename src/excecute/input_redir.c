/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	validate_input_file(const char *filename)
{
	if (access(filename, F_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: No such file or directory\n",
			filename);
		return (1);
	}
	if (access(filename, R_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n", filename);
		return (1);
	}
	return (0);
}

int	open_input_file(const char *filename)
{
	int	fd;

	if (validate_input_file(filename))
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf_stderr("minishell: %s: Failed to open file\n", filename);
		return (-1);
	}
	return (fd);
}

int	handle_input_redirection(t_redir *redir)
{
	int	fd;
	int	saved_stdin;

	if (!redir || !redir->filename)
		return (0);
	fd = open_input_file(redir->filename);
	if (fd == -1)
		return (1);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
	{
		close(fd);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		close(saved_stdin);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_input_redirections(t_cmd *cmd)
{
	t_redir	*current;
	int		result;

	if (!cmd)
		return (0);
	current = cmd->infiles;
	while (current)
	{
		result = handle_input_redirection(current);
		if (result)
			return (result);
		current = current->next;
	}
	return (0);
}

int	restore_input(int saved_fd)
{
	if (saved_fd != -1)
	{
		dup2(saved_fd, STDIN_FILENO);
		close(saved_fd);
	}
	return (0);
}
