#include "../include/minishell.h"

static int	handle_input_redirection(t_redir *redir, int *stdin_backup)
{
	int	fd;

	if (access(redir->filename, F_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: No such file or directory\n",
			redir->filename);
		return (1);
	}
	if (access(redir->filename, R_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n",
			redir->filename);
		return (1);
	}
	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	if (*stdin_backup == -1)
		*stdin_backup = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_output_redirection(t_redir *redir, int *stdout_backup)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (redir->type == REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	if (*stdout_backup == -1)
		*stdout_backup = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	process_redirections(t_cmd *cmd, int *stdin_backup,
		int *stdout_backup)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_INPUT)
		{
			if (handle_input_redirection(redir, stdin_backup))
				return (1);
		}
		else if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
		{
			if (handle_output_redirection(redir, stdout_backup))
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

static void	restore_file_descriptors(int stdin_backup, int stdout_backup)
{
	if (stdin_backup != -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
	if (stdout_backup != -1)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
}

int	execute_with_redirections(t_cmd *cmd, t_shell *shell)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;
	int	has_redir_error;

	stdin_backup = -1;
	stdout_backup = -1;
	status = 0;
	has_redir_error = 0;
	if (cmd->redirs)
		has_redir_error = process_redirections(cmd, &stdin_backup,
				&stdout_backup);
	if (cmd->heredoc > 0 && !has_redir_error)
	{
		if (stdin_backup == -1)
			stdin_backup = dup(STDIN_FILENO);
		dup2(cmd->heredoc, STDIN_FILENO);
		close(cmd->heredoc);
	}
	else if (cmd->heredoc > 0)
		close(cmd->heredoc);
	if (!has_redir_error && cmd->argv && cmd->argv[0]
		&& cmd->argv[0][0] != '\0')
	{
		if (is_builtin(cmd->argv[0]))
			status = execute_builtin(cmd, shell);
		else
			status = execute_external(cmd, shell);
	}
	else if (has_redir_error)
		status = 1;
	restore_file_descriptors(stdin_backup, stdout_backup);
	return (status);
}

int	execute_command(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	if (cmd->argv && cmd->argv[0] && cmd->argv[0][0] == '\0')
	{
		shift_argv(&cmd->argv);
		if (!cmd->argv[0])
			return (0);
	}
	return (execute_with_redirections(cmd, shell));
}
