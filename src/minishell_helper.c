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
	char	buffer[4096];
	ssize_t	bytes_read;
	int		i;

	i = 0;
	while (i < (int)(sizeof(buffer) - 1))
	{
		bytes_read = read(STDIN_FILENO, &buffer[i], 1);
		if (bytes_read <= 0)
		{
			if (i == 0)
				return (NULL);
			break ;
		}
		if (buffer[i] == '\n')
			break ;
		i++;
	}
	buffer[i] = '\0';
	return (ft_strdup(buffer));
}

char	*get_input_and_handle_signals(t_shell *shell)
{
	char		*line;
	static char	*saved_line;

	saved_line = NULL;
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
