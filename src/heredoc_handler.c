/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:36:37 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 15:35:11 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

char	*setup_heredoc_delimiter(char *delimiter, int *expand)
{
	char	*tmp_delim;

	*expand = 1;
	tmp_delim = NULL;
	if (delimiter[0] == '\'' || delimiter[0] == '"')
	{
		*expand = 0;
		tmp_delim = ft_strtrim(delimiter, "\"'");
		if (!tmp_delim)
		{
			perror("malloc");
			return (NULL);
		}
		return (tmp_delim);
	}
	return (delimiter);
}

void	write_heredoc_line(int writefd, char *line, int expand, t_shell *shell)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_variables(line, shell);
		free(line);
		if (!expanded)
		{
			close(writefd);
			exit(1);
		}
		write(writefd, expanded, ft_strlen(expanded));
		write(writefd, "\n", 1);
		free(expanded);
	}
	else
	{
		write(writefd, line, ft_strlen(line));
		write(writefd, "\n", 1);
		free(line);
	}
}

static int	handle_heredoc_eof(char *line)
{
	if (!line)
	{
		ft_putstr_fd(
			"minishell: warning: here-document delimited by end-of-file\n",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int	handle_heredoc_delimiter(char *line, char *delimiter, int expand)
{
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		if (!expand)
		{
			free(delimiter);
			return (1);
		}
		free(delimiter);
		return (1);
	}
	return (0);
}
