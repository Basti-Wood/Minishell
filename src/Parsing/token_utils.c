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

void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->str)
		free(token->str);
	free(token);
}

void	free_token_list(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free_token(current);
		current = next;
	}
}

t_token	*create_new_token(char *str, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	if (str)
		new_token->str = ft_strdup(str);
	else
		new_token->str = NULL;
	if (str && !new_token->str)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

void	append_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
	new_token->prev = current;
}

void	link_tokens(t_token **head, t_token **current, t_token *new_tokens)
{
	t_token	*last;

	last = new_tokens;
	while (last->next)
		last = last->next;
	if (*current)
		(*current)->next = new_tokens;
	else
		*head = new_tokens;
	*current = last;
}
