#include "../include/minishell.h"

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

static void	execute_child_builtin(t_cmd *current, t_shell *shell)
{
	exit(execute_builtin(current, shell));
}

static void	execute_child_command(t_cmd *current, t_shell *shell)
{
	if (!current->argv || !current->argv[0] || current->argv[0][0] == '\0')
		exit(0);
	if (is_builtin(current->argv[0]))
		execute_child_builtin(current, shell);
	else
		execute_child_external(current, shell);
}

void	setup_child_process(t_child_data *data)
{
	int		redir_status;
	t_redir	*r;
	int		has_input_redir;

	signal(SIGPIPE, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	has_input_redir = 0;
	r = data->current->redirs;
	while (r)
	{
		if (r->type == REDIR_INPUT)
			has_input_redir = 1;
		r = r->next;
	}
	if (!has_input_redir || data->i == 0)
		setup_child_pipes(data->i, data->cmd_count, data->pipes);
	else
	{
		if (data->i > 0)
			close(data->pipes[data->i - 1][0]);
		if (data->i < data->cmd_count - 1)
			dup2(data->pipes[data->i][1], STDOUT_FILENO);
		close_all_child_pipes(data->pipes, data->cmd_count);
	}
	redir_status = handle_redirections(data->current);
	if (redir_status != 0)
		exit(1);
	if (data->current->argv && data->current->argv[0])
		execute_child_command(data->current, data->shell);
	exit(0);
}