/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:45:53 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static int	setup_pipeline_resources(t_cmd *cmds, t_shell *shell,
			t_fork_data *fork_data)
{
	int		**pipes;
	pid_t	*pids;
	int		cmd_count;

	cmd_count = count_commands(cmds);
	if (cmd_count < 2)
		return (execute_command(cmds, shell));
	if (allocate_resources(&pipes, &pids, cmd_count))
		return (1);
	fork_data->cmds = cmds;
	fork_data->pipes = pipes;
	fork_data->pids = pids;
	fork_data->cmd_count = cmd_count;
	fork_data->shell = shell;
	return (0);
}

int	execute_pipeline(t_cmd *cmds, t_shell *shell)
{
	t_fork_data	fork_data;
	int			setup_result;

	setup_result = setup_pipeline_resources(cmds, shell, &fork_data);
	if (setup_result != 0)
		return (setup_result);
	if (fork_and_execute_children(&fork_data))
	{
		cleanup_resources(fork_data.pipes, fork_data.pids, fork_data.cmd_count);
		return (1);
	}
	close_all_pipes(fork_data.pipes, fork_data.cmd_count);
	wait_for_children(fork_data.pids, fork_data.cmd_count, shell);
	cleanup_resources(fork_data.pipes, fork_data.pids, fork_data.cmd_count);
	return (shell->exit_status);
}
