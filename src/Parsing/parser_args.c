/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:31:52 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 19:43:03 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	add_argument(t_cmd *cmd, char *arg, int *argc)
{
	cmd->argv = realloc(cmd->argv, (*argc + 2) * sizeof(char *));
	if (!cmd->argv)
		return (0);
	cmd->argv[*argc] = remove_quote_markers(arg);
	(*argc)++;
	cmd->argv[*argc] = NULL;
	return (1);
}

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

static void	handle_syntax_error(t_shell *shell, char *token_str)
{
	ft_fprintf_stderr("minishell: syntax error near unexpected token `%s'\n",
		token_str);
	shell->exit_status = 258;
}

int	handle_redirection(t_token **tokens, t_cmd *cmd, t_shell *shell)
{
	if ((*tokens)->type == INPUT && (*tokens)->next)
		return (handle_input_redir(tokens, cmd));
	if (((*tokens)->type == TRUNC || (*tokens)->type == APPEND)
		&& (*tokens)->next)
		return (handle_output_redir(tokens, cmd));
	if ((*tokens)->type == HEREDOC && (*tokens)->next)
		return (handle_heredoc_redir(tokens, cmd, shell));
	handle_syntax_error(shell, (*tokens)->str);
	return (0);
}
