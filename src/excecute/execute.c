/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:06 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:04:07 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <errno.h>

int	execute_command(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	if (cmd->argv && cmd->argv[0] && cmd->argv[0][0] == '\0')
	{
		shift_argv(&cmd->argv);
		if (!cmd->argv || !cmd->argv[0])
			return (0);
	}
	return (execute_with_redirections(cmd, shell));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 16:00:00 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_with_redirections(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	result;

	if (!cmd || !shell)
		return (1);
	if (!cmd->argv || !cmd->argv[0])
		return (0);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("dup");
		return (1);
	}
	if (handle_redirections_in_order(cmd) == -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	if (is_builtin(cmd->argv[0]))
		result = execute_builtin(cmd, shell);
	else
		result = execute_external_command(cmd, shell);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (result);
}

static int	handle_single_redirection(t_redir *redir)
{
	int	fd;

	if (!redir)
		return (0);
	if (redir->type == REDIR_INPUT)
	{
		if (access(redir->filename, F_OK) == -1)
		{
			ft_fprintf_stderr("minishell: %s: No such file or directory\n",
				redir->filename);
			return (-1);
		}
		if (access(redir->filename, R_OK) == -1)
		{
			ft_fprintf_stderr("minishell: %s: Permission denied\n",
				redir->filename);
			return (-1);
		}
		fd = open(redir->filename, O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			return (-1);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			return (-1);
		}
		close(fd);
	}
	else if (redir->type == REDIR_OUTPUT)
	{
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_fprintf_stderr("minishell: %s: Permission denied\n",
				redir->filename);
			return (-1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			return (-1);
		}
		close(fd);
	}
	else if (redir->type == REDIR_APPEND)
	{
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_fprintf_stderr("minishell: %s: Permission denied\n",
				redir->filename);
			return (-1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			return (-1);
		}
		close(fd);
	}
	else if (redir->type == REDIR_HEREDOC)
	{
		// Heredoc is handled elsewhere, just use the fd if available
		return (0);
	}
	return (0);
}

int	handle_redirections_in_order(t_cmd *cmd)
{
	t_redir	*current;

	if (!cmd)
		return (0);
	if (cmd->heredoc > 0)
	{
		if (dup2(cmd->heredoc, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			close(cmd->heredoc);
			return (-1);
		}
		close(cmd->heredoc);
		cmd->heredoc = -1;
	}
	current = cmd->redirs;
	while (current)
	{
		if (handle_single_redirection(current) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}

int	execute_external_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*executable;
	char	**env_array;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	executable = find_executable(cmd->argv[0], &shell->env_list);
	if (!executable)
	{
		ft_fprintf_stderr("minishell: %s: command not found\n", cmd->argv[0]);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		if (ft_strchr(executable, '/'))
			free(executable);
		return (1);
	}
	if (pid == 0)
	{
		env_array = env_list_to_array(&shell->env_list);
		if (!env_array)
		{
			if (ft_strchr(executable, '/'))
				free(executable);
			exit(1);
		}
		execve(executable, cmd->argv, env_array);
		perror("execve");
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (ft_strchr(executable, '/'))
		free(executable);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
