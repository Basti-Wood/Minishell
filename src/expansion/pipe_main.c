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

static int	fork_and_execute_children(t_fork_data *data)
{
	int		i;
	t_cmd	*current;

	current = data->cmds;
	i = 0;
	while (i < data->cmd_count && current)
	{
		if (execute_single_child(data, i, current))
			return (1);
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
	if (allocate_resources(&pipes, &pids, cmd_count))
		return (1);
	if (fork_and_execute_children(cmds, pipes, pids, cmd_count, shell))
		return (1);
	status = wait_for_children(pids, cmd_count);
	cleanup_resources(pipes, pids, cmd_count);
	return (status);
}
