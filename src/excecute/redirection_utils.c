#include "../../include/minishell.h"

int apply_input_redir(t_redir *redir, int *saved_stdin)
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

int apply_output_redir(t_redir *redir, int *saved_stdout)
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
		return (apply_input_redir(redir, saved_stdin));
	if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
		return (apply_output_redir(redir, saved_stdout));
	return (0);
}

int handle_redirections_in_order(t_cmd *cmd)
{
	t_redir *current;
	int saved_stdin;
	int saved_stdout;
	int input_failed;
	int flags;
	int fd;

	if (!cmd)
		return (0);
	input_failed = 0;
	saved_stdin = -1;
	saved_stdout = -1;
	current = cmd->redirs;
	while (current)
	{
		if (current->type == REDIR_OUTPUT || current->type == REDIR_APPEND)
		{
			flags = (current->type == REDIR_APPEND) ?
				(O_WRONLY | O_CREAT | O_APPEND) :
				(O_WRONLY | O_CREAT | O_TRUNC);
			fd = open(current->filename, flags, 0644);
			if (fd >= 0)
			{
				if (saved_stdout == -1)
					saved_stdout = dup(STDOUT_FILENO);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			else
			{
				perror(current->filename);
				return (1);
			}
		}
		else if (current->type == REDIR_INPUT)
		{
			if (process_single_redir(current, &saved_stdin, &saved_stdout) != 0)
				input_failed = 1;
		}
		current = current->next;
	}
	if (cmd->heredoc > 0)
	{
		if (dup2(cmd->heredoc, STDIN_FILENO) == -1)
			return (1);
		close(cmd->heredoc);
	}
	return (input_failed);
}

int execute_with_redirections(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (1);
	if (handle_redirections_in_order(cmd) != 0)
	{
		shell->exit_status = 1;
		return (1);
	}
	if (!cmd->argv || !cmd->argv[0])
		return (0);
	return (execute_external_command(cmd, shell));
}
