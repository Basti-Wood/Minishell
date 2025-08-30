/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child-extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:45:01 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	execute_child_builtin(t_cmd *current, t_shell *shell)
{
	exit(execute_builtin(current, shell));
}

static void	execute_child_command(t_cmd *current, t_shell *shell)
{
	if (!current->argv || !current->argv[0] || current->argv[0][0] == '\0')
		exit(0);
	if (is_builtin(current->argv[0]))
		execute_child_builtin(current, shell);
	else
		execute_child_external(current, shell);
}

void	setup_child_process(t_child_data *data)
{
	int	redir_status;

	signal(SIGPIPE, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_pipes(data->i, data->cmd_count, data->pipes);
	redir_status = handle_redirections(data->current);
	if (redir_status != 0)
		exit(1);
	if (data->current->argv && data->current->argv[0])
		execute_child_command(data->current, data->shell);
	exit(0);
}
