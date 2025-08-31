/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	validate_args(char **args)
{
	if (args[1] && args[2])
	{
		ft_fprintf_stderr("minishell: cd: too many arguments\n");
		return (1);
	}
	return (0);
}

static char	*get_target_path(char **args, t_env_list *env_list)
{
	char	*home;

	if (!args[1])
	{
		home = get_env_value(env_list, "HOME");
		if (!home)
		{
			ft_fprintf_stderr("minishell: cd: HOME not set\n");
			return (NULL);
		}
		return (home);
	}
	return (args[1]);
}

static void	update_pwd_vars(t_env_list *env_list, char *old_pwd, char *new_pwd)
{
	if (old_pwd)
		set_env_value(env_list, "OLDPWD", old_pwd);
	if (new_pwd)
	{
		set_env_value(env_list, "PWD", new_pwd);
		free(new_pwd);
	}
}

int	builtin_cd(char **args, t_env_list *env_list)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	if (validate_args(args))
		return (1);
	old_pwd = get_env_value(env_list, "PWD");
	path = get_target_path(args, env_list);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	update_pwd_vars(env_list, old_pwd, new_pwd);
	return (0);
}
