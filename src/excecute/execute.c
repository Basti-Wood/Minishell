#include "../include/minishell.h"
#include <errno.h>

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (builtin_echo(cmd->argv));
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd->argv, shell->envp));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (builtin_env(shell->envp));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd->argv, shell->envp));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd->argv, shell->envp));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd->argv));
	return (1);
}

static char	*check_path_access(char *cmd)
{
	struct stat	st;

	if (access(cmd, F_OK) == 0)
	{
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
			return (NULL);
		return (ft_strdup(cmd));
	}
	return (NULL);
}

static char	*search_in_paths(char *cmd, char **paths)
{
	int		i;
	size_t	path_len;
	size_t	cmd_len;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		path_len = ft_strlen(paths[i]);
		cmd_len = ft_strlen(cmd);
		full_path = malloc(path_len + cmd_len + 2);
		if (!full_path)
			return (NULL);
		ft_strcpy(full_path, paths[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, t_env_list *env_list)
{
	char	*path_env;
	char	**paths;
	char	*result;
	int		i;

	if (!cmd || !env_list)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (check_path_access(cmd));
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(cmd, paths);
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (result);
}
