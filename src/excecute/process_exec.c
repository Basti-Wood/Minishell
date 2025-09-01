/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:37:19 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <errno.h>

void	try_shell_script(char *executable, t_cmd *cmd, char **env_array)
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
