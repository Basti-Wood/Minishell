/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_regular_token(t_token **tokens, t_cmd *cmd, int *argc)
{
	char	*cleaned_str;

	if (!tokens || !*tokens || !(*tokens)->str || !cmd || !argc)
		return (-1);
	cleaned_str = remove_quote_markers((*tokens)->str);
	if (!cleaned_str)
		return (-1);
	if (!add_argument(cmd, cleaned_str, argc))
	{
		free(cleaned_str);
		return (-1);
	}
	free(cleaned_str);
	return (1);
}

int	process_token(t_token **tokens, t_cmd *cmd, int *argc, t_shell *shell)
{
	int	tokens_consumed;

	if (!tokens || !*tokens || !cmd || !argc)
		return (-1);
	if ((*tokens)->type == CMD || (*tokens)->type == ARG)
		return (process_regular_token(tokens, cmd, argc));
	if (is_redirection_token((*tokens)->type))
	{
		tokens_consumed = handle_redirection(tokens, cmd, shell);
		return (tokens_consumed);
	}
	return (1);
}

void	advance_tokens(t_token **tokens, int count)
{
	int	i;

	if (!tokens || !*tokens || count <= 0)
		return ;
	i = 0;
	while (i < count && *tokens)
	{
		*tokens = (*tokens)->next;
		i++;
	}
}

t_cmd	*init_and_process_cmd(t_token **tokens, t_shell *shell)
{
	t_cmd	*new_cmd;
	int		argc;
	int		tokens_consumed;

	argc = 0;
	new_cmd = init_new_cmd();
	if (!new_cmd)
		return (NULL);
	while (*tokens && (*tokens)->type != PIPE && (*tokens)->type != END)
	{
		tokens_consumed = process_token(tokens, new_cmd, &argc, shell);
		if (tokens_consumed <= 0)
		{
			free_cmds(new_cmd);
			return (NULL);
		}
		advance_tokens(tokens, tokens_consumed);
	}
	if (argc == 0 && !new_cmd->redirs)
	{
		free_cmds(new_cmd);
		return (NULL);
	}
	return (new_cmd);
}

t_cmd	*process_command_tokens(t_token **tokens, t_shell *shell)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (init_and_process_cmd(tokens, shell));
}
