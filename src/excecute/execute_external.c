#include "../include/minishell.h"
#include <errno.h>

static int	check_executable_errors(char *cmd)
{
	struct stat	st;

	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		{
			ft_fprintf_stderr("minishell: %s: Is a directory\n", cmd);
			return (126);
		}
		else if (access(cmd, F_OK) != 0)
		{
			ft_fprintf_stderr("minishell: %s: No such file or directory\n",
				cmd);
			return (127);
		}
		else if (access(cmd, X_OK) != 0)
		{
			ft_fprintf_stderr("minishell: %s: Permission denied\n", cmd);
			return (126);
		}
	}
	return (0);
}

static void	handle_execve_error(char *executable, char **cmd_argv,
		char **env_array)
{
	if (errno == ENOEXEC)
	{
		char	*sh_argv[3];

		sh_argv[0] = "/bin/sh";
		sh_argv[1] = executable;
		sh_argv[2] = NULL;
		execve("/bin/sh", sh_argv, env_array);
		ft_fprintf_stderr("minishell: %s: Exec format error\n", executable);
		exit(126);
	}
	perror("execve");
	exit(126);
}

static int	execute_child_process(char *executable, t_cmd *cmd,
		char **env_array)
{
	if (execve(executable, cmd->argv, env_array) == -1)
		handle_execve_error(executable, cmd->argv, env_array);
	return (126);
}

static int	wait_for_child(pid_t pid, char *executable, char **env_array)
{
	int	status;
	int	i;

	waitpid(pid, &status, 0);
	free(executable);
	i = 0;
	while (env_array[i])
		free(env_array[i++]);
	free(env_array);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*executable;
	char	**env_array;
	pid_t	pid;
	int		error_status;

	if (!cmd || !cmd->argv || !cmd->argv[0] || !shell)
		return (1);
	if (cmd->argv[0][0] == '\0')
	{
		shift_argv(&cmd->argv);
		if (!cmd->argv[0])
			return (0);
	}
	error_status = check_executable_errors(cmd->argv[0]);
	if (error_status)
		return (error_status);
	executable = find_executable(cmd->argv[0], shell->envp);
	if (!executable)
	{
		ft_fprintf_stderr("minishell: %s: command not found\n", cmd->argv[0]);
		return (127);
	}
	env_array = env_list_to_array(shell->envp);
	if (!env_array)
	{
		free(executable);
		return (1);
	}
	pid = fork();
	if (pid == 0)
		execute_child_process(executable, cmd, env_array);
	else if (pid > 0)
		return (wait_for_child(pid, executable, env_array));
	perror("fork");
	free(executable);
	if (env_array)
	{
		error_status = 0;
		while (env_array[error_status])
			free(env_array[error_status++]);
		free(env_array);
	}
	return (1);
}
