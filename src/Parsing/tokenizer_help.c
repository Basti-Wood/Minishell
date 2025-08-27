/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 16:00:00 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token	*create_single_token(char *element,
				t_shell *shell, t_token *prev)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = expand_variables(element, shell);
	if (!new_token->str)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = EMPTY;
	new_token->next = NULL;
	new_token->prev = prev;
	return (new_token);
}

static void	link_token_to_list(t_token **head,
				t_token **current, t_token *new_token)
{
	if (*current)
		(*current)->next = new_token;
	else
		*head = new_token;
	*current = new_token;
}

t_token	*create_token(char **elements, t_shell *shell)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	int		i;

	if (!elements || !shell)
		return (NULL);
	head = NULL;
	current = NULL;
	i = 0;
	while (elements[i])
	{
		new_token = create_single_token(elements[i], shell, current);
		if (!new_token)
		{
			free_tokens(head);
			return (NULL);
		}
		link_token_to_list(&head, &current, new_token);
		i++;
	}
	return (head);
}

static t_token_type	get_token_type(char *str, int is_first)
{
	if (ft_strcmp(str, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(str, "<") == 0)
		return (INPUT);
	else if (ft_strcmp(str, ">") == 0)
		return (TRUNC);
	else if (ft_strcmp(str, ">>") == 0)
		return (APPEND);
	else if (ft_strcmp(str, "<<") == 0)
		return (HEREDOC);
	else if (is_first)
		return (CMD);
	else
		return (ARG);
}

t_token	*assign_token_types(t_token *tokens)
{
	t_token	*current;
	int		is_first;

	if (!tokens)
		return (NULL);
	current = tokens;
	is_first = 1;
	while (current)
	{
		if (current->type == EMPTY && current->str)
		{
			current->type = get_token_type(current->str, is_first);
			if (current->type == CMD)
				is_first = 0;
		}
		if (current->type == PIPE)
			is_first = 1;
		current = current->next;
	}
	return (tokens);
}
