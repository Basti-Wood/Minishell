/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 16:00:00 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	has_pipe(t_cmd *cmds)
{
	if (!cmds)
		return (0);
	return (cmds->next != NULL);
}

static int	handle_signal_status(int sig)
{
	if (sig == SIGINT)
		return (130);
	else if (sig == SIGQUIT)
	{
		write(STDERR_FILENO, "Quit\n", 5);
		return (131);
	}
	else if (sig == SIGPIPE)
		return (0);
	else
		return (128 + sig);
}

static int	get_exit_status(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		return (handle_signal_status(sig));
	}
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1);
}

void	wait_for_children(pid_t *pids, int cmd_count, t_shell *shell)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < cmd_count)
	{
		if (waitpid(pids[i], &status, 0) > 0)
		{
			if (i == cmd_count - 1)
				last_status = get_exit_status(status);
		}
		i++;
	}
	shell->exit_status = last_status;
}
