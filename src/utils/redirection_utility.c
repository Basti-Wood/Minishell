/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utility_functions.c                    :+:      :+:    :+:   */
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
		return (-1);
	if (redir->type != REDIR_INPUT)
		return (-1);
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
	if (*fd != -1)
		close(*fd);
	*fd = open(redir->filename, O_RDONLY);
	if (*fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (0);
}

int	process_output_redir(t_redir *redir, int *fd)
{
	if (!redir || !redir->filename)
		return (-1);
	if (redir->type != REDIR_OUTPUT && redir->type != REDIR_APPEND)
		return (-1);
	if (*fd != -1)
		close(*fd);
	if (redir->type == REDIR_OUTPUT)
		*fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		*fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n",
			redir->filename);
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

static int	validate_single_redir(t_redir *redir)
{
	if (!redir)
		return (0);
	if (redir->type == REDIR_INPUT)
	{
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
	}
	else if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
	{
		char	*dir;
		char	*last_slash;
		int		result;

		dir = ft_strdup(redir->filename);
		if (!dir)
			return (-1);
		last_slash = ft_strrchr(dir, '/');
		result = 0;
		if (last_slash)
		{
			*last_slash = '\0';
			if (access(dir, W_OK) == -1)
			{
				ft_fprintf_stderr("minishell: %s: Permission denied\n", dir);
				result = -1;
			}
		}
		else
		{
			if (access(".", W_OK) == -1)
			{
				ft_fprintf_stderr("minishell: Permission denied\n");
				result = -1;
			}
		}
		free(dir);
		return (result);
	}
	return (0);
}

int	validate_redir_list(t_redir *redirs)
{
	t_redir	*current;

	if (!redirs)
		return (0);
	current = redirs;
	while (current)
	{
		if (validate_single_redir(current) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
