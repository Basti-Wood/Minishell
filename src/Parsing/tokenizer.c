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
		new_token = malloc(sizeof(t_token));
		if (!new_token)
		{
			free_tokens(head);
			return (NULL);
		}
		new_token->str = expand_variables(elements[i], shell);
		new_token->type = EMPTY;
		new_token->next = NULL;
		new_token->prev = current;
		if (current)
			current->next = new_token;
		else
			head = new_token;
		current = new_token;
		i++;
	}
	return (head);
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
			if (ft_strcmp(current->str, "|") == 0)
				current->type = PIPE;
			else if (ft_strcmp(current->str, "<") == 0)
				current->type = INPUT;
			else if (ft_strcmp(current->str, ">") == 0)
				current->type = TRUNC;
			else if (ft_strcmp(current->str, ">>") == 0)
				current->type = APPEND;
			else if (ft_strcmp(current->str, "<<") == 0)
				current->type = HEREDOC;
			else if (is_first)
			{
				current->type = CMD;
				is_first = 0;
			}
			else
				current->type = ARG;
		}
		if (current->type == PIPE)
			is_first = 1;
		current = current->next;
	}
	return (tokens);
}

