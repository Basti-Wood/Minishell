/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 16:42:52 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_redirection_token(t_token_type type)
{
	return (type == TRUNC || type == APPEND || type == INPUT || type == HEREDOC);
}

int	handle_redirection(t_token **tokens, t_cmd *cmd, t_shell *shell)
{
	t_token		*current;
	t_redir		*redir;
	t_token		*next;

	(void)shell;  // Prevent unused parameter warning
	current = *tokens;
	if (!current || !current->next)
		return (1);
	next = current->next;

	if (current->type == TRUNC)
		redir = create_redir(next->str, REDIR_OUTPUT);
	else if (current->type == APPEND)
		redir = create_redir(next->str, REDIR_APPEND);
	else if (current->type == INPUT)
		redir = create_redir(next->str, REDIR_INPUT);
	else if (current->type == HEREDOC)
	{
		redir = create_redir(next->str, REDIR_HEREDOC);
		cmd->heredoc = 1;
	}
	else
		return (1);

	if (!redir)
		return (1);

	add_redirection(cmd, redir);
	*tokens = next->next;
	return (0);
}
