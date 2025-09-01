/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:05:32 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:05:33 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	write_heredoc_line(char *content, int pipe_fd)
{
	write(pipe_fd, content, ft_strlen(content));
	write(pipe_fd, "\n", 1);
	return (0);
}

int	process_heredoc_line(char *line, t_heredoc_data *data)
{
	char	*expanded;

	if (ft_strcmp(line, data->delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (data->expand)
	{
		expanded = expand_variables(line, data->shell);
		free(line);
		if (!expanded)
			return (-1);
		write_heredoc_line(expanded, data->pipe_fd);
		free(expanded);
	}
	else
	{
		write_heredoc_line(line, data->pipe_fd);
		free(line);
	}
	return (0);
}
