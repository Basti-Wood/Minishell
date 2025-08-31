/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:05:32 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:05:33 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	minishell(env);
	return (0);
}

void	minishell(char **env)
{
	t_shell	shell;
	char	*line;

	init_main_shell(&shell, env);
	line = get_input_and_handle_signals(&shell);
	while (line)
	{
		handle_shell_input(line, &shell);
		line = get_input_and_handle_signals(&shell);
	}
	if (shell.env_list.head)
		free_env_list(&shell.env_list);
}
