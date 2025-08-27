/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 16:00:00 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token	*create_expanded_token(char *str, t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(str);
	if (!new_token->str)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

static void	insert_token_after(t_token *current, t_token *new_token)
{
	new_token->next = current->next;
	new_token->prev = current;
	if (current->next)
		current->next->prev = new_token;
	current->next = new_token;
}

static t_token	*replace_with_expanded(t_token *current, char **expanded)
{
	t_token	*first;
	t_token	*new_token;
	t_token	*last;
	int		i;

	first = create_expanded_token(expanded[0], current->type);
	if (!first)
		return (current);
	first->prev = current->prev;
	first->next = current->next;
	if (current->prev)
		current->prev->next = first;
	if (current->next)
		current->next->prev = first;
	last = first;
	i = 1;
	while (expanded[i])
	{
		new_token = create_expanded_token(expanded[i], current->type);
		if (new_token)
			insert_token_after(last, new_token);
		last = new_token;
		i++;
	}
	free(current->str);
	free(current);
	return (first);
}

static void	free_expanded_array(char **expanded)
{
	int	i;

	if (!expanded)
		return ;
	i = 0;
	while (expanded[i])
	{
		free(expanded[i]);
		i++;
	}
	free(expanded);
}

t_token	*expand_wildcards_in_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;
	t_token	*head;
	char	**expanded;

	head = tokens;
	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->type == ARG && has_wildcard(current->str))
		{
			expanded = expand_wildcard(current->str);
			if (expanded)
			{
				current = replace_with_expanded(current, expanded);
				if (current->prev == NULL)
					head = current;
				free_expanded_array(expanded);
			}
		}
		current = next;
	}
	return (head);
}
