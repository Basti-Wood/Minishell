/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:16:16 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 13:16:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int		pipefd[2];
	char	*processed_delim;
	int		expand;
	int		result;

	processed_delim = setup_heredoc_delimiter(delimiter, &expand);
	if (!processed_delim)
		return (-1);
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		cleanup_heredoc_delimiter(processed_delim, delimiter);
		return (-1);
	}
	result = fork_heredoc_process(pipefd, processed_delim, expand, shell);
	cleanup_heredoc_delimiter(processed_delim, delimiter);
	return (result);
}
