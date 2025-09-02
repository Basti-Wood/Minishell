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
	int		exit_status;

	(void)argc;
	(void)argv;
	exit_status = minishell(env);
	return (exit_status);
}

int	minishell(char **env)
{
	t_shell	shell;
	char	*line;
	int		exit_status;

	init_main_shell(&shell, env);
	line = get_input_and_handle_signals(&shell);
	while (line && !shell.should_exit)
	{
		handle_shell_input(line, &shell);
		if (shell.should_exit)
			break ;
		line = get_input_and_handle_signals(&shell);
	}
	exit_status = shell.exit_status;
	if (shell.env_list.head)
		free_env_list(&shell.env_list);
	return (exit_status);
}
