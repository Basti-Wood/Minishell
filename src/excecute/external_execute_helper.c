/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_execute_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:17:16 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/26 14:17:23 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	handle_exec_failure(char *executable, char **env_array)
{
	perror("execve");
	cleanup_process_resources(executable, env_array);
	exit(127);
}

static void	try_shell_script(char *executable, t_cmd *cmd, char **env_array)
{
	char	**new_argv;
	int		argc;

	argc = get_argc(cmd->argv);
	new_argv = create_sh_argv(executable, cmd->argv, argc);
	if (!new_argv)
	{
		cleanup_process_resources(executable, env_array);
		exit(127);
	}
	if (execve("/bin/sh", new_argv, env_array) == -1)
	{
		perror("execve /bin/sh");
		free_string_array(new_argv);
		cleanup_process_resources(executable, env_array);
		exit(127);
	}
}
