#include "../../include/minishell.h"

int apply_output_redirection(t_redir *redir, int flags)
{
	int	fd;

	if (!redir || !redir->filename)
		return (1);
	if (redir->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n",
			redir->filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int handle_input_redir_exec(t_redir *redir)
{
	int fd;

	if (!redir->filename)
		return (1);
	if (access(redir->filename, F_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: No such file or directory\n", 
			redir->filename);
		return (1);
	}
	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n",
			redir->filename);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int handle_single_redirection(t_redir *redir)
{
	if (!redir)
		return (0);
	if (redir->type == REDIR_INPUT)
		return (handle_input_redir_exec(redir));
	else
		return (apply_output_redirection(redir, 0));
}

int apply_input_redir_safe(t_redir *redir, int *saved_stdin)
{
	int fd;

	if (!redir || !redir->filename)
		return (0);
	if (access(redir->filename, F_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: No such file or directory\n",
			redir->filename);
		return (1);
	}
	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	if (*saved_stdin == -1)
		*saved_stdin = dup(STDIN_FILENO);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int apply_output_redir_safe(t_redir *redir, int *saved_stdout)
{
	int fd;
	int flags;

	if (!redir || !redir->filename)
		return (1);
	if (redir->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	if (*saved_stdout == -1)
		*saved_stdout = dup(STDOUT_FILENO);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int process_single_redir(t_redir *redir, int *saved_stdin, int *saved_stdout)
{
	if (!redir)
		return (0);
	if (redir->type == REDIR_HEREDOC)
		return (0);
	if (redir->type == REDIR_INPUT)
		return (apply_input_redir_safe(redir, saved_stdin));
	if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
		return (apply_output_redir_safe(redir, saved_stdout));
	return (0);
}
