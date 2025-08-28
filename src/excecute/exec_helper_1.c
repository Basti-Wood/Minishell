/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:06 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 17:18:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_with_redirections(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	result;

	if (!cmd || !shell)
		return (1);
	if (!cmd->argv || !cmd->argv[0])
		return (0);
	if (save_file_descriptors(&saved_stdin, &saved_stdout) == -1)
		return (1);
	if (handle_redirections_in_order(cmd) == -1)
	{
		restore_file_descriptors(saved_stdin, saved_stdout);
		return (1);
	}
	if (is_builtin(cmd->argv[0]))
		result = execute_builtin(cmd, shell);
	else
		result = execute_external_command(cmd, shell);
	restore_file_descriptors(saved_stdin, saved_stdout);
	return (result);
}

int	handle_input_redirection(t_redir *redir)
{
	int	fd;

	if (!redir || !redir->filename)
		return (0);
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
	if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
	{
		perror("input redirection");
		if (fd != -1)
			close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	apply_output_redirection(t_redir *redir, int flags)
{
	int	fd;

	fd = open(redir->filename, flags, 0644);
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
	return (0);
}

int	handle_single_redirection(t_redir *redir)
{
	if (!redir)
		return (0);
	if (redir->type == REDIR_INPUT)
		return (handle_input_redirection(redir));
	else if (redir->type == REDIR_OUTPUT)
		return (apply_output_redirection(redir, O_WRONLY | O_CREAT | O_TRUNC));
	else if (redir->type == REDIR_APPEND)
		return (apply_output_redirection(redir, O_WRONLY | O_CREAT | O_APPEND));
	else if (redir->type == REDIR_HEREDOC)
		return (0);
	return (0);
}
