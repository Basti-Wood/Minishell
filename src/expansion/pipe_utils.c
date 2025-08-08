/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:43:36 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 17:43:44 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	has_pipe(t_cmd *cmds)
{
	if (!cmds)
		return (0);
	return (cmds->next != NULL);
}

static int	check_file_access(char *filename)
{
	if (access(filename, F_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: No such file or directory\n",
			filename);
		return (-1);
	}
	if (access(filename, R_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n", filename);
		return (-1);
	}
	return (0);
}

static int	handle_input_redir(t_redir *redir)
{
	int	fd;

	if (check_file_access(redir->filename) == -1)
		return (-1);
	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}
