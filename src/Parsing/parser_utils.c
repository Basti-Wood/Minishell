/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:29:52 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/18 13:30:15 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*init_new_cmd(void)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	ft_bzero(new_cmd, sizeof(t_cmd));
	new_cmd->argv = ft_calloc(1, sizeof(char *));
	new_cmd->heredoc = -1;
	new_cmd->infiles = NULL;
	new_cmd->outfiles = NULL;
	new_cmd->redirs = NULL;
	return (new_cmd);
}

void	add_to_ordered_redirs(t_cmd *cmd, t_redir *redir)
{
	t_redir	**last;

	last = &cmd->redirs;
	while (*last)
		last = &(*last)->next;
	*last = redir;
}

void	add_to_infiles(t_cmd *cmd, t_redir *redir)
{
	t_redir	**last;

	last = &cmd->infiles;
	while (*last)
		last = &(*last)->next;
	*last = redir;
}
