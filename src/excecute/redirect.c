#include "../include/minishell.h"

static int	process_input_files(t_cmd *cmd)
{
	int		fd;
	t_redir	*redir;
	int		tmp_fd;

	fd = -1;
	redir = cmd->infiles;
	while (redir)
	{
		if (redir->type == REDIR_INPUT)
		{
			tmp_fd = open(redir->filename, O_RDONLY);
			if (tmp_fd == -1)
			{
				perror(redir->filename);
				if (fd != -1)
					close(fd);
				return (-1);
			}
			if (fd != -1)
				close(fd);
			fd = tmp_fd;
		}
		redir = redir->next;
	}
	return (fd);
}

int	handle_input_redirection(t_cmd *cmd)
{
	int	fd;

	fd = -1;
	if (cmd->infiles)
	{
		fd = process_input_files(cmd);
		if (fd == -1)
			return (-1);
		if (fd != -1)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2 input");
				close(fd);
				return (-1);
			}
			close(fd);
		}
	}
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

static int	process_output_files(t_cmd *cmd)
{
	int		fd;
	t_redir	*redir;
	int		flags;
	int		tmp_fd;

	fd = -1;
	redir = cmd->outfiles;
	while (redir)
	{
		flags = O_WRONLY | O_CREAT;
		if (redir->type == REDIR_APPEND)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		tmp_fd = open(redir->filename, flags, 0644);
		if (tmp_fd == -1)
		{
			perror(redir->filename);
			if (fd != -1)
				close(fd);
			return (-1);
		}
		if (fd != -1)
			close(fd);
		fd = tmp_fd;
		redir = redir->next;
	}
	return (fd);
}

int	handle_output_redirection(t_cmd *cmd)
{
	int	fd;

	fd = -1;
	if (cmd->outfiles)
	{
		fd = process_output_files(cmd);
		if (fd == -1)
			return (-1);
		if (fd != -1)
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 output");
				close(fd);
				return (-1);
			}
			close(fd);
		}
	}
	return (0);
}

int	validate_input_file(const char *filename)
{
	if (access(filename, F_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: No such file or directory\n",
			filename);
		return (0);
	}
	if (access(filename, R_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n", filename);
		return (0);
	}
	return (1);
}
