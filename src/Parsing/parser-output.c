/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-output.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:31:19 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/18 13:31:30 by seftekha         ###   ########.fr       */
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
	t_redir			*outfile_redir;

	type = get_output_type((*tokens)->type);
	*tokens = (*tokens)->next;
	redir = create_redir(remove_quote_markers((*tokens)->str), type);
	if (!redir)
		return (0);
	add_to_ordered_redirs(cmd, redir);
	outfile_redir = create_redir(ft_strdup(redir->filename), type);
	if (!outfile_redir)
		return (0);
	add_to_outfiles(cmd, outfile_redir);
	return (1);
}

int	handle_heredoc_redir(t_token **tokens, t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;
	char	*delimiter;

	*tokens = (*tokens)->next;
	if (cmd->heredoc > 0)
		close(cmd->heredoc);
	delimiter = remove_quote_markers((*tokens)->str);
	cmd->heredoc = handle_heredoc(delimiter, shell);
	free(delimiter);
	if (cmd->heredoc == -1)
		return (0);
	redir = create_redir(NULL, REDIR_HEREDOC);
	if (!redir)
		return (0);
	add_to_ordered_redirs(cmd, redir);
	return (1);
}
