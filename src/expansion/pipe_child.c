/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:44:32 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 17:44:41 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	close_all_child_pipes(int **pipes, int cmd_count)
{
	int	j;

	j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

static void	setup_child_pipes(int i, int cmd_count, int **pipes)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < cmd_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_all_child_pipes(pipes, cmd_count);
}

static void	execute_child_external(t_cmd *current, t_shell *shell)
{
	char	*executable;
	char	**env_array;

	executable = find_executable(current->argv[0], shell->envp);
	if (!executable)
	{
		ft_fprintf_stderr("minishell: %s: command not found\n",
			current->argv[0]);
		exit(127);
	}
	env_array = env_list_to_array(shell->envp);
	if (!env_array)
	{
		free(executable);
		exit(1);
	}
	execve(executable, current->argv, env_array);
	perror("execve");
	exit(126);
}
