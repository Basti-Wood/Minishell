/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper_2_fixed.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:06 by seftekha          #+#    #+#             */
/*   Updated: Fixed version for double free issues      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_argc(char **argv)
{
	int	count;

	count = 0;
	if (!argv)
		return (0);
	while (argv[count])
		count++;
	return (count);
}

int	save_file_descriptors(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("dup");
		return (-1);
	}
	return (0);
}

void	restore_file_descriptors(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

int	execute_command(t_cmd *cmd, t_shell *shell)
{
	int saved_stdin;
	int saved_stdout;
	int result;

	printf("[DEBUG] execute_command: Entered\n");
	if (!cmd)
	{
		printf("[DEBUG] execute_command: Null command\n");
		return (0);
	}
	printf("[DEBUG] execute_command: Command %s\n", cmd->argv ? cmd->argv[0] : "NULL");

	if (cmd->argv && cmd->argv[0] && cmd->argv[0][0] == '\0')
	{
		printf("[DEBUG] execute_command: Empty command, shifting arguments\n");
		shift_argv(&cmd->argv);
		if (!cmd->argv || !cmd->argv[0])
		{
			printf("[DEBUG] execute_command: No arguments after shift\n");
			return (0);
		}
	}

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("[DEBUG] execute_command: Failed to save file descriptors");
		return (1);
	}

	if (handle_redirections_in_order(cmd) != 0)
	{
		printf("[DEBUG] execute_command: Failed to handle redirections\n");
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}

	printf("[DEBUG] execute_command: Redirections set up, executing command '%s'\n", cmd->argv[0]);
	if (is_builtin(cmd->argv[0]))
	{
		printf("[DEBUG] execute_command: Executing builtin '%s'\n", cmd->argv[0]);
		result = execute_builtin(cmd, shell);
	}
	else
	{
		printf("[DEBUG] execute_command: Executing external command '%s'\n", cmd->argv[0]);
		result = execute_external_command(cmd, shell);
	}
	printf("[DEBUG] execute_command: Command finished with status %d\n", result);

	printf("[DEBUG] execute_command: Restoring file descriptors\n");
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);

	return result;
}

int	handle_redirections_in_order(t_cmd *cmd)
{
	t_redir	*current;

	if (!cmd)
		return (0);
	
	// Process all redirections in order first
	current = cmd->redirs;
	while (current)
	{
		if (handle_single_redirection(current) == -1)
			return (-1);
		current = current->next;
	}
	
	// Handle heredoc after other redirections (it has lower priority)
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
	
	return (0);
}
