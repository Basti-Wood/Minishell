/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-output_fixed.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:31:19 by seftekha          #+#    #+#             */
/*   Updated: Fixed for memory issues                   ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_redir_type	get_output_type(t_token_type token_type)
{
	if (token_type == APPEND)
		return (REDIR_APPEND);
	return (REDIR_OUTPUT);
}

int	handle_output_redir(t_token **tokens, t_cmd *cmd)
{
	t_redir_type	type;
	t_redir			*redir;
	char			*filename;

	type = get_output_type((*tokens)->type);
	*tokens = (*tokens)->next;
	if (!*tokens || !(*tokens)->str)
		return (0);
	
	// Create a clean filename without quote markers
	filename = remove_quote_markers((*tokens)->str);
	if (!filename)
		return (0);
	
	// Create only ONE redirection for the ordered list
	redir = create_redir(filename, type);
	if (!redir)
	{
		free(filename);
		return (0);
	}
	add_to_ordered_redirs(cmd, redir);
	
	return (1);
}

int	handle_heredoc_redir(t_token **tokens, t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;
	char	*delimiter;

	*tokens = (*tokens)->next;
	if (!*tokens || !(*tokens)->str)
		return (0);
	
	// Close previous heredoc if exists
	if (cmd->heredoc > 0)
		close(cmd->heredoc);
	
	delimiter = remove_quote_markers((*tokens)->str);
	if (!delimiter)
		return (0);
	
	cmd->heredoc = handle_heredoc(delimiter, shell);
	free(delimiter);
	
	if (cmd->heredoc == -1)
		return (0);
	
	redir = create_redir(NULL, REDIR_HEREDOC);
	if (!redir)
	{
		close(cmd->heredoc);
		cmd->heredoc = -1;
		return (0);
	}
	add_to_ordered_redirs(cmd, redir);
	
	return (1);
}
