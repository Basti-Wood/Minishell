/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_assign.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*create_redir(char *filename, t_redir_type type)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->filename = filename;
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

int	validate_redir_filename(t_token *next_token)
{
	if (!next_token || !next_token->str)
	{
		ft_fprintf_stderr("minishell: syntax error unexpected token\n");
		return (0);
	}
	return (1);
}

int	handle_input_redir(t_token **tokens, t_cmd *cmd)
{
	t_redir	*redir;
	char	*filename;
	t_token	*next_token;

	if (!tokens || !*tokens || !cmd)
		return (-1);
	next_token = (*tokens)->next;
	if (!validate_redir_filename(next_token))
		return (-1);
	filename = remove_quote_markers(next_token->str);
	if (!filename)
		return (-1);
	redir = create_redir(filename, REDIR_INPUT);
	if (!redir)
	{
		free(filename);
		return (-1);
	}
	add_to_ordered_redirs(cmd, redir);
	return (2);
}
