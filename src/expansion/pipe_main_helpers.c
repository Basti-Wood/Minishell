/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_main_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:37:10 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 18:38:05 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	execute_single_child(t_fork_data *data, int i, t_cmd *current)
{
	data->pids[i] = fork();
	if (data->pids[i] == -1)
	{
		perror("fork");
		return (1);
	}
	if (data->pids[i] == 0)
		setup_child_process(i, data->cmd_count, data->pipes,
			current, data->shell);
	return (0);
}

static int	allocate_resources(int **pipes, pid_t **pids, int cmd_count)
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

static void	cleanup_resources(int **pipes, pid_t *pids, int cmd_count)
{
	close_all_pipes(pipes, cmd_count);
	cleanup_pipes(pipes, cmd_count);
	free(pids);
}
