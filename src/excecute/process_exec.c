/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:37:19 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 14:37:40 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <errno.h>

static void	child_process(char *executable, t_cmd *cmd, char **env_array)
{
	if (execve(executable, cmd->argv, env_array) == -1)
	{
		if (errno == ENOEXEC)
		{
			try_shell_script(executable, cmd, env_array);
			ft_fprintf_stderr("minishell: %s: Exec format error\n", executable);
			exit(126);
		}
		perror("execve");
		exit(126);
	}
}

static int	parent_process(pid_t pid, char *executable, char **env_array)
{
	int	status;

	waitpid(pid, &status, 0);
	cleanup_exec_resources(executable, env_array);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else
		return (1);
}

static void	try_shell_script(char *executable, t_cmd *cmd, char **env_array)
{
	char	*sh_argv[4];
	char	**new_argv;
	int		argc;

	sh_argv[0] = "/bin/sh";
	sh_argv[1] = executable;
	sh_argv[2] = NULL;
	if (cmd->argv[1])
	{
		argc = get_argc(cmd->argv);
		new_argv = create_sh_argv(executable, cmd->argv, argc);
		execve("/bin/sh", new_argv, env_array);
		free(new_argv);
	}
	else
		execve("/bin/sh", sh_argv, env_array);
}

static void	cleanup_exec_resources(char *executable, char **env_array)
{
	int	i;

	free(executable);
	if (env_array)
	{
		i = 0;
		while (env_array[i])
			free(env_array[i++]);
		free(env_array);
	}
}
