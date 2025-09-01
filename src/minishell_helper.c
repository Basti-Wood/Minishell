/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:05:38 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:05:39 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*read_from_stdin(void)
{
	char	*buffer;
	char	*line;
	size_t	len;
	ssize_t	read_result;

	buffer = NULL;
	len = 0;
	read_result = getline(&buffer, &len, stdin);
	if (read_result == -1)
	{
		if (buffer)
			free(buffer);
		return (NULL);
	}
	if (buffer[read_result - 1] == '\n')
		buffer[read_result - 1] = '\0';
	line = ft_strdup(buffer);
	free(buffer);
	return (line);
}

char	*get_input_and_handle_signals(t_shell *shell)
{
	char		*line;
	static char	*saved_line = NULL;

	setup_signals(shell);
	if (saved_line)
	{
		line = saved_line;
		saved_line = NULL;
		return (line);
	}
	if (isatty(STDIN_FILENO))
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			return (NULL);
		}
	}
	else
		line = read_from_stdin();
	return (line);
}
