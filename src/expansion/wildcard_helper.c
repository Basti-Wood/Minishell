/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 16:00:00 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	link_first_token(t_token *first, t_token *current)
{
	first->prev = current->prev;
	first->next = current->next;
	if (current->prev)
		current->prev->next = first;
	if (current->next)
		current->next->prev = first;
}

static t_token	*add_remaining_tokens(t_token *first, char **expanded)
{
	t_token	*new_token;
	t_token	*last;
	int		i;

	last = first;
	i = 1;
	while (expanded[i])
	{
		new_token = create_expanded_token(expanded[i], first->type);
		if (new_token)
			insert_token_after(last, new_token);
		last = new_token;
		i++;
	}
	return (first);
}

t_token	*replace_with_expanded(t_token *current, char **expanded)
{
	t_token	*first;

	first = create_expanded_token(expanded[0], current->type);
	if (!first)
		return (current);
	link_first_token(first, current);
	add_remaining_tokens(first, expanded);
	free(current->str);
	free(current);
	return (first);
}
