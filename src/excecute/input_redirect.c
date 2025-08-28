#include "../../include/minishell.h"

static int process_redir_loop(t_redir *redir, int *fd)
{
	t_redir *current;
	int result;
	
	current = redir;
	while (current)
	{
		result = process_input_redir(current, fd);
		if (result == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}

int handle_input_redirections(t_cmd *cmd)
{
	int fd;
	int result;

	fd = -1;
	if (!cmd)
		return (0);
	if (cmd->infiles)
	{
		result = process_redir_loop(cmd->infiles, &fd);
		if (result == -1)
			return (-1);
	}
	if (apply_input_fd(fd) == -1)
		return (-1);
	if (cmd->heredoc > 0)
	{
		if (dup2(cmd->heredoc, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			return (-1);
		}
		close(cmd->heredoc);
	}
	return (0);
}