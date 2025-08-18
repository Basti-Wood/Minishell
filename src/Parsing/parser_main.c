/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:32:15 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/18 13:32:57 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_token(t_token **tokens, t_cmd *cmd, int *argc, t_shell *shell)
{
	if ((*tokens)->type == CMD || (*tokens)->type == ARG)
	{
		if (!add_argument(cmd, (*tokens)->str, argc))
			return (0);
	}
	else if (is_redirection_token((*tokens)->type))
	{
		if (!handle_redirection(tokens, cmd, shell))
			return (0);
	}
	return (1);
}

t_cmd	*process_command_tokens(t_token **tokens, t_shell *shell)
{
	t_cmd	*new_cmd;
	int		argc;

	new_cmd = init_new_cmd();
	if (!new_cmd)
		return (NULL);
	argc = 0;
	while (*tokens && (*tokens)->type != PIPE && (*tokens)->type != END)
	{
		if (!process_token(tokens, new_cmd, &argc, shell))
		{
			free_cmds(new_cmd);
			return (NULL);
		}
		*tokens = (*tokens)->next;
	}
	if (argc == 0 && !new_cmd->redirs)
	{
		free_cmds(new_cmd);
		return (NULL);
	}
	return (new_cmd);
}
