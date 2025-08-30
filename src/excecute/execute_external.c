#include "../../include/minishell.h"

int	create_env_array(t_shell *shell, char ***env_array)
{
	t_env_node	*current;
	char		*env_str;
	char		*tmp;
	int			env_count;

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

void	free_env_array(char **env_array)
{
	int	i;

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

static void	handle_command_error(char *cmd)
{
	struct stat	st;

	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
			ft_fprintf_stderr("minishell: %s: Is a directory\n", cmd);
		else
			ft_fprintf_stderr("minishell: %s: No such file or directory\n", cmd);
	}
	else
		ft_fprintf_stderr("minishell: %s: command not found\n", cmd);
}

static void	handle_exec_child(t_cmd *cmd, t_shell *shell)
{
	char		*executable;
	char		**env_array;

	executable = find_executable(cmd->argv[0], &shell->env_list);
	if (!executable)
	{
		handle_command_error(cmd->argv[0]);
		exit(127);
	}
	if (create_env_array(shell, &env_array) != 0)
	{
		free(executable);
		exit(1);
	}
	execute_process(executable, cmd, env_array);
}

int	execute_external_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	pid = fork();
	if (pid == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: fork: %s\n", strerror(errno));
		return (1);
	}
	if (pid == 0)
		handle_exec_child(cmd, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}