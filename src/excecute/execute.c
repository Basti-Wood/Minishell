/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:06 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:04:07 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <errno.h>

int	execute_command(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	if (cmd->argv && cmd->argv[0] && cmd->argv[0][0] == '\0')
	{
		shift_argv(&cmd->argv);
		if (!cmd->argv || !cmd->argv[0])
			return (0);
	}
	return (execute_with_redirections(cmd, shell));
}
