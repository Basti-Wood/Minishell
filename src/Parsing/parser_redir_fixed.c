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
	char	*filename;

	*tokens = (*tokens)->next;
	if (!*tokens || !(*tokens)->str)
		return (0);
	
	// Create a clean filename without quote markers
	filename = remove_quote_markers((*tokens)->str);
	if (!filename)
		return (0);
	
	// Create only ONE redirection for the ordered list
	redir = create_redir(filename, REDIR_INPUT);
	if (!redir)
	{
		free(filename);
		return (0);
	}
	add_to_ordered_redirs(cmd, redir);
	
	return (1);
}

// Note: Remove the add_to_infiles and add_to_outfiles functions as they're no longer needed
