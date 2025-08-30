#include "../../include/minishell.h"

int save_fds(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin == -1)
		return 1;
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdout == -1)
	{
		close(*saved_stdin);
		return 1;
	}
	return 0;
}

void restore_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

int apply_redir(t_redir *redir)
{
	int fd;
	int flags;
	if (!redir)
		return 0;
	if (redir->type == REDIR_INPUT)
	{
		if (!redir->filename)
			return 1;
		if (access(redir->filename, F_OK) == -1)
			return 1;
		fd = open(redir->filename, O_RDONLY);
		if (fd == -1)
			return 1;
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			return 1;
		}
		close(fd);
		return 0;
	}
	if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
	{
		if (!redir->filename)
			return 1;
		if (redir->type == REDIR_APPEND)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(redir->filename, flags, 0644);
		if (fd == -1)
			return 1;
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			return 1;
		}
		close(fd);
		return 0;
	}
	return 0;
}

int apply_redirs_recursive(t_redir *redir)
{
	int result;
	if (!redir)
		return 0;
	result = apply_redir(redir);
	if (result)
		return result;
	return apply_redirs_recursive(redir->next);
}

int handle_redirections(t_cmd *cmd)
{
	int result;
	if (!cmd)
		return 0;
	result = apply_redirs_recursive(cmd->redirs);
	if (result)
		return result;
	if (cmd->heredoc > 0)
	{
		if (dup2(cmd->heredoc, STDIN_FILENO) == -1)
		{
			close(cmd->heredoc);
			return 1;
		}
		close(cmd->heredoc);
	}
	return 0;
}
