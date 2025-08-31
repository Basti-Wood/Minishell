/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_pipe_syntax(t_token *tokens, t_cmd *current, t_shell *shell)
{
	(void)tokens;
	if (!current)
	{
		ft_fprintf_stderr("minishell: syntax error near unexpected token`|'\n");
		shell->exit_status = 258;
		return (0);
	}
	return (1);
}

void	add_cmd_to_list(t_cmd **head, t_cmd **current, t_cmd *new_cmd)
{
	if (!*head)
		*head = new_cmd;
	else
		(*current)->next = new_cmd;
	*current = new_cmd;
}

t_cmd	*parse_tokens(t_token *tokens, t_shell *shell)
{
	t_cmd	*cmd_list;
	t_token	*current;

	if (!tokens || !shell)
		return (NULL);
	current = tokens;
	cmd_list = parse_pipeline(&current, shell);
	return (cmd_list);
}
