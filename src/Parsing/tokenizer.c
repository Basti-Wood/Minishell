/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 16:00:00 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*tokenize(const char *input, t_shell *shell)
{
	char	**elements;
	t_token	*tokens;

	if (!input || !shell)
		return (NULL);
	elements = ft_split_quotes(input);
	if (!elements)
		return (NULL);
	tokens = create_token(elements, shell);
	free_argv(elements);
	if (!tokens)
		return (NULL);
	tokens = assign_token_types(tokens);
	return (tokens);
}

static int	is_empty_token(t_token *token)
{
	if (!token || !token->str)
		return (1);
	if (token->str[0] == '\0')
		return (1);
	return (0);
}

static void	remove_token_from_list(t_token **head, t_token *to_remove)
{
	if (!head || !*head || !to_remove)
		return ;
	if (*head == to_remove)
	{
		*head = to_remove->next;
		if (to_remove->next)
			to_remove->next->prev = NULL;
	}
	else
	{
		if (to_remove->prev)
			to_remove->prev->next = to_remove->next;
		if (to_remove->next)
			to_remove->next->prev = to_remove->prev;
	}
	free(to_remove->str);
	free(to_remove);
}

t_token	*handle_empty_expansions(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current)
	{
		next = current->next;
		if (is_empty_token(current) && current->type != PIPE
			&& current->type != INPUT && current->type != TRUNC
			&& current->type != APPEND && current->type != HEREDOC)
		{
			if (current == tokens)
				tokens = next;
			remove_token_from_list(&tokens, current);
		}
		current = next;
	}
	return (tokens);
}

int	has_quote_markers(char *str)
{
	while (str && *str)
	{
		if (*str == '\001' || *str == '\002')
			return (1);
		if (*str == '\003' || *str == '\004')
			return (1);
		str++;
	}
	return (0);
}
