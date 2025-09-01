/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:15 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_exec_error(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (errno == EACCES)
			ft_fprintf_stderr("minishell: %s: Permission denied\n", cmd);
		else
			ft_fprintf_stderr("minishell: %s: No such file or directory\n",
				cmd);
	}
	else
		ft_fprintf_stderr("minishell: %s: command not found\n", cmd);
}

static void	handle_execve_failure(t_cmd *cmd, char *executable,
			char **env_array)
{
	if (errno == ENOEXEC)
	{
		try_shell_script(executable, cmd, env_array);
		exit(0);
	}
	else if (errno == EACCES)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n", cmd->argv[0]);
		cleanup_process_resources(executable, env_array);
		exit(126);
	}
	print_exec_error(cmd->argv[0]);
	cleanup_process_resources(executable, env_array);
	exit(127);
}

void	execute_process(char *executable, t_cmd *cmd, char **env_array)
{
	struct stat	st;

	if (stat(executable, &st) == -1)
	{
		print_exec_error(cmd->argv[0]);
		cleanup_process_resources(executable, env_array);
		exit(127);
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_fprintf_stderr("minishell: %s: Is a directory\n", cmd->argv[0]);
		cleanup_process_resources(executable, env_array);
		exit(126);
	}
	execve(executable, cmd->argv, env_array);
	handle_execve_failure(cmd, executable, env_array);
}

void	cleanup_process_resources(char *executable, char **env_array)
{
	if (executable)
		free(executable);
	if (env_array)
		free_string_array(env_array);
}
