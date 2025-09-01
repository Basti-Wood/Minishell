/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child_extra_extra.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/09/01 14:18:02 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_input_redirections(t_cmd *cmd)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_INPUT)
			return (1);
		r = r->next;
	}
	return (0);
}

void	handle_child_pipes(t_child_data *data, int has_input_redir)
{
	if (data->i > 0 && !has_input_redir)
		dup2(data->pipes[data->i - 1][0], STDIN_FILENO);
	if (data->i < data->cmd_count - 1)
		dup2(data->pipes[data->i][1], STDOUT_FILENO);
	close_all_child_pipes(data->pipes, data->cmd_count);
}

void	setup_child_process(t_child_data *data)
{
	int	redir_status;
	int	has_input_redir;

	setup_child_signals();
	has_input_redir = check_input_redirections(data->current);
	handle_child_pipes(data, has_input_redir);
	redir_status = handle_redirections(data->current);
	if (redir_status != 0)
		exit(1);
	if (data->current->argv && data->current->argv[0])
		execute_child_command(data->current, data->shell);
	exit(0);
}
