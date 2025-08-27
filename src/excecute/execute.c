/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:06 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:04:07 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <errno.h>

static void	execute_child_process(char *executable, t_cmd *cmd, t_shell *shell)
{
	char	**env_array;

	env_array = env_list_to_array(&shell->env_list);
	if (!env_array)
	{
		if (ft_strchr(executable, '/'))
			free(executable);
		exit(1);
	}
	execve(executable, cmd->argv, env_array);
	perror("execve");
	exit(127);
}

static void	cleanup_executable(char *executable)
{
	if (ft_strchr(executable, '/'))
		free(executable);
}

static int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_external_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*executable;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	executable = find_executable(cmd->argv[0], &shell->env_list);
	if (!executable)
	{
		ft_fprintf_stderr("minishell: %s: command not found\n", cmd->argv[0]);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		cleanup_executable(executable);
		return (1);
	}
	if (pid == 0)
		execute_child_process(executable, cmd, shell);
	waitpid(pid, &status, 0);
	cleanup_executable(executable);
	return (get_exit_status(status));
}
