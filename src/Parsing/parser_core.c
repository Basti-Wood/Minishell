/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:33:27 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 19:47:04 by seftekha         ###   ########.fr       */
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

static t_cmd	*handle_pipe_token(t_token **tokens, t_cmd *current,
	t_shell *shell, t_cmd *head)
{
	if (!check_pipe_syntax(*tokens, current, shell))
	{
		free_cmds(head);
		return (NULL);
	}
	*tokens = (*tokens)->next;
	return (head);
}

static t_cmd	*handle_command_token(t_token **tokens, t_cmd **current,
	t_shell *shell, t_cmd *head)
{
	t_cmd	*new_cmd;

	new_cmd = process_command_tokens(tokens, shell);
	if (!new_cmd && shell->exit_status == 130)
	{
		free_cmds(head);
		return (NULL);
	}
	if (new_cmd)
		add_cmd_to_list(&head, current, new_cmd);
	return (head);
}

t_cmd	*parse_tokens(t_token *tokens, t_shell *shell)
{
	t_cmd	*head;
	t_cmd	*current;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
			head = handle_pipe_token(&tokens, current, shell, head);
		else if (tokens->type == CMD || tokens->type == ARG
			|| is_redirection_token(tokens->type))
			head = handle_command_token(&tokens, &current, shell, head);
		else
			tokens = tokens->next;
		if (!head)
			return (NULL);
		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
	}
	return (head);
}
