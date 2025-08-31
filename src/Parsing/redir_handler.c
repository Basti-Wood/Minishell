/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_redirection_token(t_token_type type)
{
	if (type == INPUT)
		return (1);
	if (type == TRUNC)
		return (1);
	if (type == APPEND)
		return (1);
	if (type == HEREDOC)
		return (1);
	return (0);
}

int	handle_redirection(t_token **tokens, t_cmd *cmd, t_shell *shell)
{
	if (!tokens || !*tokens || !cmd)
		return (-1);
	if ((*tokens)->type == INPUT)
		return (handle_input_redir(tokens, cmd));
	if ((*tokens)->type == TRUNC || (*tokens)->type == APPEND)
		return (handle_output_redir(tokens, cmd));
	if ((*tokens)->type == HEREDOC)
		return (handle_heredoc_redir(tokens, cmd, shell));
	return (-1);
}
