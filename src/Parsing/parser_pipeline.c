/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	add_cmd_to_pipeline(t_cmd **cmd_list,
								t_cmd **current_cmd, t_cmd *new_cmd)
{
	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		*current_cmd = new_cmd;
	}
	else
	{
		(*current_cmd)->next = new_cmd;
		*current_cmd = new_cmd;
	}
}

t_cmd	*parse_pipeline(t_token **tokens, t_shell *shell)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	t_cmd	*new_cmd;

	cmd_list = NULL;
	current_cmd = NULL;
	while (*tokens && (*tokens)->type != END)
	{
		new_cmd = process_command_tokens(tokens, shell);
		if (!new_cmd)
		{
			free_cmds(cmd_list);
			return (NULL);
		}
		add_cmd_to_pipeline(&cmd_list, &current_cmd, new_cmd);
		if (*tokens && (*tokens)->type == PIPE)
			*tokens = (*tokens)->next;
	}
	return (cmd_list);
}
