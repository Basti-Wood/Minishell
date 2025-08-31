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
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGPIPE)
				ft_fprintf_stderr(" Broken pipe\n");
			if (i == cmd_count - 1)
			{
				if (WIFEXITED(status))
					last_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					last_status = 128 + WTERMSIG(status);
			}
		}
		i++;
	}
	shell->exit_status = last_status;
}
