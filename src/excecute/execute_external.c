#include "../../include/minishell.h"

int create_env_array(t_shell *shell, char ***env_array)
{
	t_env_node *current;
	char *env_str;
	char *tmp;
	int env_count;

	env_count = shell->env_list.size;
	*env_array = malloc(sizeof(char *) * (env_count + 1));
	if (!*env_array)
		return (1);
	env_count = 0;
	current = shell->env_list.head;
	while (current)
	{
		if (current->value)
			env_str = ft_strjoin(current->key, "=");
		else
			env_str = ft_strdup(current->key);
		if (env_str && current->value)
		{
			tmp = env_str;
			env_str = ft_strjoin(env_str, current->value);
			free(tmp);
		}
		if (env_str)
			(*env_array)[env_count++] = env_str;
		current = current->next;
	}
	(*env_array)[env_count] = NULL;
	return (0);
}

void free_env_array(char **env_array)
{
	int i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

int execute_external(t_cmd *cmd, t_shell *shell)
{
	char *path;
	char **env_array;
	int status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (create_env_array(shell, &env_array) != 0)
		return (1);
	path = path_search(cmd->argv[0], env_array);
	if (!path)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: command not found\n",
			cmd->argv[0]);
		free_env_array(env_array);
		return (127);
	}
	status = execve(path, cmd->argv, env_array);
	free_env_array(env_array);
	free(path);
	if (status == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n",
			cmd->argv[0], strerror(errno));
		return (126);
	}
	return (status);
}

int execute_external_command(t_cmd *cmd, t_shell *shell)
{
	pid_t pid;
	int status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
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
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
