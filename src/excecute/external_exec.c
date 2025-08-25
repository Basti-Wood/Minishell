/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:36:10 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 14:36:38 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <errno.h>

int	execute_external(t_cmd *cmd, t_shell *shell)
{
	char		*executable;
	int			path_check;

	if (!validate_external_cmd(cmd, shell))
		return (1);
	if (cmd->argv[0][0] == '\0')
	{
		shift_argv(&cmd->argv);
		if (!cmd->argv || !cmd->argv[0])
			return (0);
	}
	path_check = check_path_command(cmd->argv[0]);
	if (path_check != 0)
		return (path_check);
	executable = find_executable(cmd->argv[0], shell->envp);
	if (!executable)
	{
		ft_fprintf_stderr("minishell: %s: command not found\n", cmd->argv[0]);
		return (127);
	}
	return (fork_and_exec(executable, cmd, shell));
}

static int	validate_external_cmd(t_cmd *cmd, t_shell *shell)
{
	return (cmd && cmd->argv && cmd->argv[0] && shell);
}

static int	check_path_command(char *cmd)
{
	struct stat	st;

	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		{
			ft_fprintf_stderr("minishell: %s: Is a directory\n", cmd);
			return (126);
		}
		else if (access(cmd, F_OK) != 0)
		{
			ft_fprintf_stderr
					("minishell: %s: No such file or directory\n", cmd);
			return (127);
		}
		else if (access(cmd, X_OK) != 0)
		{
			ft_fprintf_stderr("minishell: %s: Permission denied\n", cmd);
			return (126);
		}
	}
	return (0);
}

static int	fork_and_exec(char *executable, t_cmd *cmd, t_shell *shell)
{
	char	**env_array;
	pid_t	pid;
	int		status;

	env_array = env_list_to_array(shell->envp);
	if (!env_array)
	{
		free(executable);
		return (1);
	}
	pid = fork();
	if (pid == 0)
		child_process(executable, cmd, env_array);
	else if (pid > 0)
		status = parent_process(pid, executable, env_array);
	else
	{
		perror("fork");
		cleanup_exec_resources(executable, env_array);
		return (1);
	}
	return (status);
}
