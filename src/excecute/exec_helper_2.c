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
