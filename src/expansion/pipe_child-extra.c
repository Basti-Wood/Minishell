/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child-extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:45:01 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 17:45:08 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	execute_child_builtin(t_cmd *current, t_shell *shell)
{
	exit(execute_builtin(current, shell));
}

static void	execute_child_command(t_cmd *current, t_shell *shell)
{
	if (current->argv && current->argv[0] && current->argv[0][0] != '\0')
	{
		if (is_builtin(current->argv[0]))
			execute_child_builtin(current, shell);
		else
			execute_child_external(current, shell);
	}
	exit(0);
}

void	setup_child_process(t_child_data *data)
{
	signal(SIGPIPE, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_pipes(data->pipes, data->cmd_count, data->i);
	if (handle_redirections_in_order(data->current) == -1)
		exit(1);
	execute_child_command(data->current, data->shell);
}
