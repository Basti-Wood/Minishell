/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 16:42:52 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	int		status;

	char **env_array;
	t_env_node *current;
	int env_count;

	printf("[DEBUG] execute_external: Entered\n");
	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		printf("[DEBUG] execute_external: Invalid command structure\n");
		return (1);
	}
	printf("[DEBUG] execute_external: Executing '%s'\n", cmd->argv[0]);

	// Convert env_list to array
	env_count = shell->env_list.size;
	env_array = malloc(sizeof(char *) * (env_count + 1));
	if (!env_array)
		return (1);

	env_count = 0;
	current = shell->env_list.head;
	while (current)
	{
		char *env_str;
		if (current->value)
			env_str = ft_strjoin(current->key, "=");
		else
			env_str = ft_strdup(current->key);
		
		if (env_str && current->value)
		{
			char *tmp = env_str;
			env_str = ft_strjoin(env_str, current->value);
			free(tmp);
		}
		
		if (env_str)
			env_array[env_count++] = env_str;
		current = current->next;
	}
	env_array[env_count] = NULL;

	printf("[DEBUG] execute_external: Searching for command in PATH\n");
	path = path_search(cmd->argv[0], env_array);
	printf("[DEBUG] execute_external: path_search returned: %s\n", path ? path : "NULL");
	
	if (!path)
	{
		printf("[DEBUG] execute_external: Command not found\n");
		ft_fprintf(STDERR_FILENO, "minishell: %s: command not found\n",
			cmd->argv[0]);
		// Free the environment array and its strings
		for (int i = 0; env_array[i]; i++)
			free(env_array[i]);
		free(env_array);
		return (127);
	}

	printf("[DEBUG] execute_external: Attempting to execute at path: %s\n", path);
	status = execve(path, cmd->argv, env_array);
	// No need to free after successful execve since process is replaced
	// Only free if execve fails
	for (int i = 0; env_array[i]; i++)
		free(env_array[i]);
	free(env_array);
	free(path);
	if (status == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
			cmd->argv[0], strerror(errno));
		return (126);
	}
	return (status);
}

int	execute_external_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	printf("[DEBUG] execute_external_command: Entered\n");
	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		printf("[DEBUG] execute_external_command: Invalid command structure\n");
		return (1);
	}
	printf("[DEBUG] execute_external_command: Forking to execute '%s'\n", cmd->argv[0]);

	pid = fork();
	if (pid == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: fork: %s\n", strerror(errno));
		return (1);
	}
	if (pid == 0)
	{
		status = execute_external(cmd, shell);
		exit(status);
	}
	printf("[DEBUG] execute_external_command: Waiting for child process\n");
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		printf("[DEBUG] execute_external_command: Child exited with status %d\n", WEXITSTATUS(status));
		return (WEXITSTATUS(status));
	}
	printf("[DEBUG] execute_external_command: Child terminated abnormally\n");
	return (1);
}
