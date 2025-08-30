/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:44:32 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
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

int	setup_child_pipes(int i, int cmd_count, int **pipes)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < cmd_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_all_child_pipes(pipes, cmd_count);
	return (0);
}

static void	handle_exec_error(char *cmd, char *executable)
{
	if (ft_strchr(cmd, '/'))
		ft_fprintf_stderr("minishell: %s: No such file or directory\n", cmd);
	else
		ft_fprintf_stderr("minishell: %s: command not found\n", cmd);
	if (executable)
		free(executable);
}

static void	handle_directory_error(char *executable, char **env_array)
{
	ft_fprintf_stderr("minishell: %s: Is a directory\n", executable);
	free(executable);
	free_string_array(env_array);
	exit(126);
}

void	execute_child_external(t_cmd *current, t_shell *shell)
{
	char		*executable;
	char		**env_array;
	struct stat	st;

	executable = find_executable(current->argv[0], &shell->env_list);
	if (!executable)
	{
		handle_exec_error(current->argv[0], executable);
		exit(127);
	}
	env_array = env_list_to_array(&shell->env_list);
	if (!env_array)
	{
		free(executable);
		exit(1);
	}
	if (stat(executable, &st) == 0 && S_ISDIR(st.st_mode))
		handle_directory_error(executable, env_array);
	execve(executable, current->argv, env_array);
	if (errno == EACCES)
		ft_fprintf_stderr("minishell: %s: Permission denied\n", executable);
	else if (errno == ENOENT && ft_strchr(current->argv[0], '/'))
		ft_fprintf_stderr("minishell: %s: No such file or directory\n",
			current->argv[0]);
	else
		ft_fprintf_stderr("minishell: %s: command not found\n", executable);
	exit(127);
}