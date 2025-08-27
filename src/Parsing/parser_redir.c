/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:30:35 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:09:56 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_to_outfiles(t_cmd *cmd, t_redir *redir)
{
	t_redir	**last;

	last = &cmd->outfiles;
	while (*last)
		last = &(*last)->next;
	*last = redir;
}

t_redir	*create_redir(char *filename, t_redir_type type)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->filename = filename;
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

int	handle_input_redir(t_token **tokens, t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*infile_redir;

	*tokens = (*tokens)->next;
	redir = create_redir(remove_quote_markers((*tokens)->str), REDIR_INPUT);
	if (!redir)
		return (0);
	add_to_ordered_redirs(cmd, redir);
	infile_redir = create_redir(ft_strdup(redir->filename), REDIR_INPUT);
	if (!infile_redir)
		return (0);
	add_to_infiles(cmd, infile_redir);
	return (1);
}