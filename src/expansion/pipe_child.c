/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:44:32 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	close_all_child_pipes(int **pipes, int cmd_count)
{
	int	j;

	j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

int has_input_redirect(t_cmd *cmd)
{
	t_redir *current;

	if (!cmd || !cmd->redirs)
		return (0);
	current = cmd->redirs;
	while (current)
	{
		if (current->type == REDIR_INPUT || current->type == REDIR_HEREDOC)
			return (1);
		current = current->next;
	}
	return (cmd->heredoc > 0);
}

int has_output_redirect(t_cmd *cmd)
{
	t_redir *current;

	if (!cmd || !cmd->redirs)
		return (0);
	current = cmd->redirs;
	while (current)
	{
		if (current->type == REDIR_OUTPUT || current->type == REDIR_APPEND)
			return (1);
		current = current->next;
	}
	return (0);
}

int	setup_child_pipes(int i, int cmd_count, int **pipes)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < cmd_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_all_child_pipes(pipes, cmd_count);
	return (0);
}

void	execute_child_external(t_cmd *current, t_shell *shell)
{
	char	*executable;
	char	**env_array;
	struct stat	st;

	executable = find_executable(current->argv[0], &shell->env_list);
	if (!executable)
	{
		ft_fprintf_stderr("minishell: %s: command not found\n",
			current->argv[0]);
		exit(127);
	}
	env_array = env_list_to_array(&shell->env_list);
	if (!env_array)
	{
		free(executable);
		exit(1);
	}
	if (stat(executable, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_fprintf_stderr("minishell: %s: Is a directory\n", executable);
		free(executable);
		free_string_array(env_array);
		exit(126);
	}
	execve(executable, current->argv, env_array);
	if (errno == EACCES)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n", executable);
		exit(126);
	}
	ft_fprintf_stderr("minishell: %s: command not found\n", executable);
	exit(127);
}
