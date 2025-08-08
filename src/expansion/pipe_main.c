/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:45:53 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 17:45:58 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	close_all_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

static void	cleanup_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

static int	fork_and_execute_children(t_cmd *cmds, int **pipes,
	pid_t *pids, int cmd_count, t_shell *shell)
{
	int		i;
	t_cmd	*current;

	current = cmds;
	i = 0;
	while (i < cmd_count && current)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			return (1);
		}
		if (pids[i] == 0)
			setup_child_process(i, cmd_count, pipes, current, shell);
		current = current->next;
		i++;
	}
	return (0);
}

int	execute_pipeline(t_cmd *cmds, t_shell *shell)
{
	int		**pipes;
	pid_t	*pids;
	int		cmd_count;
	int		status;

	cmd_count = count_commands(cmds);
	if (cmd_count < 2)
		return (execute_command(cmds, shell));
	pipes = allocate_pipes(cmd_count);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pipes || !pids)
	{
		perror("malloc");
		return (1);
	}
	if (fork_and_execute_children(cmds, pipes, pids, cmd_count, shell) == 1)
		return (1);
	close_all_pipes(pipes, cmd_count);
	status = wait_for_children(pids, cmd_count);
	cleanup_pipes(pipes, cmd_count);
	free(pids);
	return (status);
}
