#include "../../include/minishell.h"


int open_output_file(const char *filename, int flags)
{
	int fd;

	if (validate_output_file(filename))
		return (-1);
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		ft_fprintf_stderr("minishell: %s: Failed to open file\n", filename);
		return (-1);
	}
	return (fd);
}

int handle_output_redirection(t_redir *redir)
{
	int fd;
	int flags;
	int saved_stdout;

	if (!redir || !redir->filename)
		return (0);
	flags = (redir->type == REDIR_APPEND) ?
		(O_WRONLY | O_CREAT | O_APPEND) :
		(O_WRONLY | O_CREAT | O_TRUNC);
	fd = open_output_file(redir->filename, flags);
	if (fd == -1)
		return (1);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		close(fd);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		close(saved_stdout);
		return (1);
	}
	close(fd);
	return (0);
}

int apply_output_redirections(t_cmd *cmd)
{
	t_redir *current;
	int result;

	if (!cmd)
		return (0);
	current = cmd->outfiles;
	while (current)
	{
		result = handle_output_redirection(current);
		if (result)
			return (result);
		current = current->next;
	}
	return (0);
}
