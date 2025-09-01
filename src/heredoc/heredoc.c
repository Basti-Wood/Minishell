/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:05:32 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:05:33 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*prepare_delimiter(char *delimiter, int *expand)
{
	char	*tmp_delim;

	*expand = 1;
	tmp_delim = NULL;
	if (delimiter[0] == SINGLE_QUOTE_START
		|| delimiter[0] == DOUBLE_QUOTE_START)
	{
		*expand = 0;
		tmp_delim = remove_quote_markers(delimiter);
		if (!tmp_delim)
		{
			perror("malloc");
			return (NULL);
		}
	}
	else
	{
		tmp_delim = ft_strdup(delimiter);
		if (!tmp_delim)
		{
			perror("malloc");
			return (NULL);
		}
	}
	return (tmp_delim);
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int		pipefd[2];
	char	*tmp_delim;
	int		expand;

	tmp_delim = prepare_delimiter(delimiter, &expand);
	if (!tmp_delim)
		return (-1);
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		free(tmp_delim);
		return (-1);
	}
	if (process_heredoc_content(tmp_delim, shell, pipefd[1], expand) == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		free(tmp_delim);
		return (-1);
	}
	close(pipefd[1]);
	free(tmp_delim);
	return (pipefd[0]);
}
