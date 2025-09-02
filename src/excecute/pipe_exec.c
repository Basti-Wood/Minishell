/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	pipe_create(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		ft_fprintf_stderr("minishell: pipe: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

void	execute_child_process(t_cmd *cmd, int *pipe_in, int *pipe_out,
		t_shell *shell)
{
	setup_child_signals();
	if (pipe_in && pipe_in[0] != -1)
	{
		dup2(pipe_in[0], STDIN_FILENO);
		close(pipe_in[0]);
		close(pipe_in[1]);
	}
	if (pipe_out && pipe_out[1] != -1)
	{
		dup2(pipe_out[1], STDOUT_FILENO);
		close(pipe_out[0]);
		close(pipe_out[1]);
	}
	handle_all_redirections(cmd);
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin(cmd, shell));
	else
		execute_process(cmd->argv[0], cmd,
			env_list_to_array(&shell->env_list));
	exit(1);
}

int	pipe_execute_command(t_cmd *cmd, int *pipe_in, int *pipe_out,
		t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_fprintf_stderr("minishell: fork: %s\n", strerror(errno));
		return (1);
	}
	if (pid == 0)
		execute_child_process(cmd, pipe_in, pipe_out, shell);
	return (pid);
}

static int	process_child_status(int status, int index, int count)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGPIPE)
		{
			if (index == count - 1)
				return (141);
		}
		else if (index == count - 1)
			return (128 + sig);
	}
	else if (WIFEXITED(status) && index == count - 1)
		return (WEXITSTATUS(status));
	return (0);
}

int	pipe_wait_for_children(pid_t *pids, int count, t_shell *shell)
{
	int	i;
	int	status;
	int	last_status;
	int	temp_status;

	i = 0;
	last_status = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		temp_status = process_child_status(status, i, count);
		if (i == count - 1)
			last_status = temp_status;
		i++;
	}
	shell->exit_status = last_status;
	return (last_status);
}
