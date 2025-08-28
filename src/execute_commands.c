/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:41:26 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 12:43:03 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execute_commands(t_cmd *cmds, t_shell *shell)
{
	if (has_pipe(cmds))
		shell->exit_status = execute_pipeline(cmds, shell);
	else
		shell->exit_status = execute_command(cmds, shell);
}

void	run_shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		handle_signal_status(shell);
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (handle_interrupted_readline(line))
			continue ;
		if (*line)
			add_history(line);
		shell->input = line;
		if (process_input(shell))
		{
			free(line);
			continue ;
		}
		free(line);
	}
}
