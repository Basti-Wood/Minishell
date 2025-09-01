/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:22:49 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_to_ordered_redirs(t_cmd *cmd, t_redir *redir)
{
	t_redir	*current;

	if (!cmd->redirs)
	{
		cmd->redirs = redir;
		return ;
	}
	current = cmd->redirs;
	while (current->next)
		current = current->next;
	current->next = redir;
}

void	add_to_infile_list(t_cmd *cmd, t_redir *redir)
{
	t_redir	*current;

	if (!cmd->infiles)
		cmd->infiles = redir;
	else
	{
		current = cmd->infiles;
		while (current->next)
			current = current->next;
		current->next = redir;
	}
}

void	add_to_outfile_list(t_cmd *cmd, t_redir *redir)
{
	t_redir	*current;

	if (!cmd->outfiles)
		cmd->outfiles = redir;
	else
	{
		current = cmd->outfiles;
		while (current->next)
			current = current->next;
		current->next = redir;
	}
}
