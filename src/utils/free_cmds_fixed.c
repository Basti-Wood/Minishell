/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds_fixed.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:00:00 by seftekha          #+#    #+#             */
/*   Updated: Fixed version to prevent double free       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		
		// Free argv array
		if (tmp->argv)
			free_argv(tmp->argv);
		
		// Free only the main redirs list (no more infiles/outfiles)
		free_redirs(tmp->redirs);
		
		// Close heredoc fd if still open
		if (tmp->heredoc > 0)
			close(tmp->heredoc);
		
		// Free the cmd structure itself
		free(tmp);
	}
}
