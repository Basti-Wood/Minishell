/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_sigint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:43:35 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 13:12:09 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_sigint(int sig)
{
	extern volatile sig_atomic_t	g_signal_status;

	g_signal_status = sig;
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

void	handle_signal_status(t_shell *shell)
{
	extern volatile sig_atomic_t	g_signal_status;

	if (g_signal_status == SIGINT)
	{
		shell->exit_status = 130;
		g_signal_status = 0;
	}
}

int	handle_interrupted_readline(char *line)
{
	extern volatile sig_atomic_t	g_signal_status;

	if (g_signal_status == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		free(line);
		return (1);
	}
	return (0);
}
