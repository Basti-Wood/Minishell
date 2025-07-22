#include "../include/minishell.h"

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

static int	prepare_heredoc_delimiter(char **delimiter, int *expand)
{
	char	*tmp_delim;

	*expand = 1;
	if ((*delimiter)[0] == '\'' || (*delimiter)[0] == '"')
	{
		*expand = 0;
		tmp_delim = ft_strtrim(*delimiter, "\"'");
		if (!tmp_delim)
		{
			perror("malloc");
			return (-1);
		}
		*delimiter = tmp_delim;
		return (1);
	}
	return (0);
}

static void	write_heredoc_line(int fd, char *line, int expand, t_shell *shell)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_variables(line, shell);
		free(line);
		if (!expanded)
		{
			close(fd);
			exit(1);
		}
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

static void	heredoc_read_loop(int fd, char *delimiter, int expand,
		t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: here-document delimited by end-of-file\n",
				STDERR_FILENO);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(fd, line, expand, shell);
	}
}

static void	heredoc_child(int fd, char *delimiter, int expand, t_shell *shell)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	close(fd);
	heredoc_read_loop(fd, delimiter, expand, shell);
	close(fd);
	exit(0);
}

static int	create_heredoc_pipe(int pipefd[2], int free_delim,
		char *delimiter)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		if (free_delim)
			free(delimiter);
		return (-1);
	}
	return (0);
}

static pid_t	fork_heredoc(int pipefd[2], int free_delim, char *delimiter)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		if (free_delim)
			free(delimiter);
	}
	return (pid);
}

static int	handle_heredoc_result(int pipefd[2], pid_t pid, int status,
		int free_delim)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipefd[0]);
		return (-1);
	}
	if (WIFEXITED(status))
		return (pipefd[0]);
	close(pipefd[0]);
	return (-1);
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	int		expand;
	int		free_delim;

	free_delim = prepare_heredoc_delimiter(&delimiter, &expand);
	if (free_delim == -1)
		return (-1);
	if (create_heredoc_pipe(pipefd, free_delim, delimiter) == -1)
		return (-1);
	pid = fork_heredoc(pipefd, free_delim, delimiter);
	if (pid == -1)
		return (-1);
	if (pid == 0)
		heredoc_child(pipefd[1], delimiter, expand, shell);
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (free_delim)
		free(delimiter);
	return (handle_heredoc_result(pipefd, pid, status, free_delim));
}
