#include "../include/minishell.h"

static int	validate_cd_args(char **args)
{
	if (args[1] && args[2])
	{
		ft_fprintf_stderr("minishell: cd: too many arguments\n");
		return (1);
	}
	return (0);
}

static char	*get_cd_path(char **args, t_env_list *env_list)
{
	char	*home;

	if (!args[1])
	{
		home = get_env_value(env_list, "HOME");
		if (!home)
		{
			ft_fprintf_stderr("cd: HOME not set\n");
			return (NULL);
		}
		return (home);
	}
	return (args[1]);
}

static int	update_pwd_vars(t_env_list *env_list, char *old_pwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		if (old_pwd)
			set_env_value(env_list, "OLDPWD", old_pwd);
		set_env_value(env_list, "PWD", new_pwd);
		free(new_pwd);
		return (0);
	}
	return (1);
}

int	builtin_cd(char **args, t_env_list *env_list)
{
	char	*path;
	char	*old_pwd;
	char	current_pwd[1024];

	if (validate_cd_args(args))
		return (1);
	if (getcwd(current_pwd, sizeof(current_pwd)) == NULL)
	{
		perror("cd: getcwd error");
		return (1);
	}
	old_pwd = get_env_value(env_list, "PWD");
	path = get_cd_path(args, env_list);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	update_pwd_vars(env_list, old_pwd);
	return (0);
}
