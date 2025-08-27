/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:45:28 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	get_child_exit_status(int child_status)
{
	if (WIFEXITED(child_status))
		return (WEXITSTATUS(child_status));
	else if (WIFSIGNALED(child_status))
		return (128 + WTERMSIG(child_status));
	return (0);
}

void	wait_for_children(pid_t *pids, int cmd_count, t_shell *shell)
{
	int	status;
	int	i;
	int	child_status;

	status = 0;
	i = 0;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &child_status, 0);
			if (i == cmd_count - 1)
				status = get_child_exit_status(child_status);
		}
		i++;
	}
	shell->exit_status = status;
}
