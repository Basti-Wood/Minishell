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
