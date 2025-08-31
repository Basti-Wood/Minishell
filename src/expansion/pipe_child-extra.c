/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child-extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 16:00:00 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_all_child_pipes(int **pipes, int cmd_count)
{
	int	j;

	j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

void	execute_child_builtin(t_cmd *current, t_shell *shell)
{
	exit(execute_builtin(current, shell));
}

void	execute_child_command(t_cmd *current, t_shell *shell)
{
	if (!current->argv || !current->argv[0] || current->argv[0][0] == '\0')
		exit(0);
	if (is_builtin(current->argv[0]))
		execute_child_builtin(current, shell);
	else
		execute_child_external(current, shell);
}

void	setup_child_signals(void)
{
	signal(SIGPIPE, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
