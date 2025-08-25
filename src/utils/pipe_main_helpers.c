/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_main_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:29:20 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 18:29:26 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct s_fork_data
{
	t_cmd	*cmds;
	int		**pipes;
	pid_t	*pids;
	int		cmd_count;
	t_shell	*shell;
}	t_fork_data;

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

