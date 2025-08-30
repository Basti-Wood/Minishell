#include "../include/minishell.h"

int	has_pipe(t_cmd *cmds)
{
	if (!cmds)
		return (0);
	return (cmds->next != NULL);
}

void	handle_broken_pipe(int status, int is_last_cmd)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGPIPE)
	{
		if (!is_last_cmd)
			ft_fprintf_stderr("minishell: Broken pipe\n");
	}
}

void	wait_for_children(pid_t *pids, int cmd_count, t_shell *shell)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		else
			handle_broken_pipe(status, 0);
		i++;
	}
	shell->exit_status = last_status;
}