#include "../../include/minishell.h"

int check_file_access(char *filename, int mode)
{
	if (access(filename, F_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: No such file or directory\n",
			filename);
		return (1);
	}
	if (access(filename, mode) == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n", filename);
		return (1);
	}
	return (0);
}

int validate_input_file_redir(t_redir *redir)
{
	if (!redir || !redir->filename)
		return (1);
	return (check_file_access(redir->filename, R_OK));
}

int validate_output_file_redir(t_redir *redir)
{
	char *dir;
	char *last_slash;
	int result;

	if (!redir || !redir->filename)
		return (1);
	dir = ft_strdup(redir->filename);
	if (!dir)
		return (1);
	last_slash = ft_strrchr(dir, '/');
	if (last_slash)
	{
		*last_slash = '\0';
		result = check_file_access(dir, W_OK);
	}
	else
		result = check_file_access(".", W_OK);
	free(dir);
	return (result);
}

int process_redir_with_validation(t_redir *redir)
{
	int fd;
	int flags;

	if (!redir || !redir->filename)
		return (1);
	if (validate_single_redir(redir) != 0)
		return (1);
	if (redir->type == REDIR_INPUT)
	{
		fd = open(redir->filename, O_RDONLY);
		if (fd == -1)
			return (1);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (redir->type == REDIR_OUTPUT)
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(redir->filename, flags, 0644);
		if (fd == -1)
			return (1);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir->type == REDIR_APPEND)
	{
		flags = O_WRONLY | O_CREAT | O_APPEND;
		fd = open(redir->filename, flags, 0644);
		if (fd == -1)
			return (1);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (0);
}

int get_argc(char **argv)
{
	int count;

	count = 0;
	if (!argv)
		return (0);
	while (argv[count])
		count++;
	return (count);
}
