/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:15 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execute_process(char *executable, t_cmd *cmd, char **env_array)
{
	if (execve(executable, cmd->argv, env_array) == -1)
	{
		if (errno == ENOEXEC)
		{
			try_shell_script(executable, cmd, env_array);
		}
		else
		{
			perror("execve");
			cleanup_process_resources(executable, env_array);
			exit(127);
		}
	}
}

void	cleanup_process_resources(char *executable, char **env_array)
{
	if (executable && ft_strchr(executable, '/'))
		free(executable);
	if (env_array)
		free_string_array(env_array);
}

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

char	**create_sh_argv(char *executable, char **argv, int argc)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (argc + 3));
	if (!new_argv)
		return (NULL);
	new_argv[0] = ft_strdup("sh");
	new_argv[1] = ft_strdup(executable);
	if (!new_argv[0] || !new_argv[1])
	{
		free_string_array(new_argv);
		return (NULL);
	}
	i = 1;
	while (i < argc)
	{
		new_argv[i + 1] = ft_strdup(argv[i]);
		if (!new_argv[i + 1])
		{
			free_string_array(new_argv);
			return (NULL);
		}
		i++;
	}
	new_argv[argc + 1] = NULL;
	return (new_argv);
}
