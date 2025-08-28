#include "../../include/minishell.h"

static void	setup_pipe_redirection(int i, int cmd_count, int **pipes)
{
	if (i > 0)
	{
		dup2(pipes[i - 1][0], STDIN_FILENO);
		close(pipes[i - 1][0]);
		close(pipes[i - 1][1]);
	}
	if (i < cmd_count - 1)
	{
		dup2(pipes[i][1], STDOUT_FILENO);
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

static void	close_pipes_in_parent(int **pipes, int cmd_count)
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

static void	wait_for_processes(pid_t *pids, int cmd_count, t_shell *shell)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
}

static int	fork_and_exec(t_cmd *cmd, int i, int cmd_count,
						  int **pipes, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		setup_pipe_redirection(i, cmd_count, pipes);
		if (apply_redirections(cmd) < 0)
			exit(1);
		if (is_builtin(cmd->argv[0]))
			exit(execute_builtin(cmd, shell));
		else
			exit(execute_external(cmd, shell));
	}
	return (pid);
}

int	execute_pipeline(t_cmd *cmds, t_shell *shell)
{
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	t_cmd	*current;
	int		i;

	cmd_count = count_commands(cmds);
	pipes = create_pipes(cmd_count);
	if (!pipes)
		return (1);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		free_pipes(pipes, cmd_count);
		return (1);
	}
	current = cmds;
	i = 0;
	while (current)
	{
		pids[i] = fork_and_exec(current, i, cmd_count, pipes, shell);
		if (pids[i] == -1)
		{
			free(pids);
			free_pipes(pipes, cmd_count);
			return (1);
		}
		current = current->next;
		i++;
	}
	close_pipes_in_parent(pipes, cmd_count);
	wait_for_processes(pids, cmd_count, shell);
	free(pids);
	free_pipes(pipes, cmd_count);
	return (shell->exit_status);
}
