/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:15 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

static void	handle_command_error(char *cmd)
{
	struct stat	st;

	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
			ft_fprintf_stderr("minishell: %s: Is a directory\n", cmd);
		else
			ft_fprintf_stderr("minishell: %s: No such file or directory\n",
				cmd);
	}
	else
		ft_fprintf_stderr("minishell: %s: command not found\n", cmd);
}

static void	handle_exec_child(t_cmd *cmd, t_shell *shell)
{
	char	*executable;
	char	**env_array;

	setup_child_signals();
	executable = find_executable(cmd->argv[0], &shell->env_list);
	if (!executable)
	{
		handle_command_error(cmd->argv[0]);
		exit(127);
	}
	if (create_env_array(shell, &env_array) != 0)
	{
		free(executable);
		exit(1);
	}
	execute_process(executable, cmd, env_array);
}

static int	get_process_exit_status(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			return (130);
		}
		else if (sig == SIGQUIT)
		{
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
			return (131);
		}
		else
			return (128 + sig);
	}
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_external_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	pid = fork();
	if (pid == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: fork: %s\n", strerror(errno));
		return (1);
	}
	if (pid == 0)
	{
		handle_exec_child(cmd, shell);
	}
	if (waitpid(pid, &status, 0) == -1)
		return (1);
	return (get_process_exit_status(status));
}
