/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_main_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:37:10 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_all_pipes(int **pipes, int cmd_count)
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

void	cleanup_pipes(int **pipes, int cmd_count)
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

int	execute_single_child(t_fork_data *data, int i, t_cmd *current)
{
	t_child_data	child_data;

	data->pids[i] = fork();
	if (data->pids[i] == -1)
	{
		perror("fork");
		return (1);
	}
	if (data->pids[i] == 0)
	{
		child_data.i = i;
		child_data.cmd_count = data->cmd_count;
		child_data.pipes = data->pipes;
		child_data.current = current;
		child_data.shell = data->shell;
		setup_child_process(&child_data);
	}
	return (0);
}

int	allocate_resources(int ***pipes, pid_t **pids, int cmd_count)
{
	*pipes = allocate_pipes(cmd_count);
	*pids = malloc(sizeof(pid_t) * cmd_count);
	if (!*pipes || !*pids)
	{
		perror("malloc");
		return (1);
	}
	return (0);
}

void	cleanup_resources(int **pipes, pid_t *pids, int cmd_count)
{
	close_all_pipes(pipes, cmd_count);
	cleanup_pipes(pipes, cmd_count);
	free(pids);
}
