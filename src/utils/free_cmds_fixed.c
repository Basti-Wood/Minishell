/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds_fixed.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:22:49 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	t_cmd	*next;

	tmp = cmds;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->argv)
		{
			free_argv(tmp->argv);
			tmp->argv = NULL;
		}
		if (tmp->redirs)
		{
			free_redirs(tmp->redirs);
			tmp->redirs = NULL;
		}
		if (tmp->heredoc > 0)
		{
			close(tmp->heredoc);
			tmp->heredoc = -1;
		}
		free(tmp);
		tmp = next;
	}
}
