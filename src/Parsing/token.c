/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:04:42 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

void	type_arg(t_token *token)
{
	if (token->str[0] == '\0')
		token->type = EMPTY;
	else if (ft_strcmp(token->str, ">") == 0)
		token->type = TRUNC;
	else if (ft_strcmp(token->str, ">>") == 0)
		token->type = APPEND;
	else if (ft_strcmp(token->str, "<") == 0)
		token->type = INPUT;
	else if (ft_strcmp(token->str, "<<") == 0)
		token->type = HEREDOC;
	else if (ft_strcmp(token->str, "|") == 0)
		token->type = PIPE;
	else if (ft_strcmp(token->str, ";") == 0)
		token->type = END;
	else if (token->prev == NULL || token->prev->type == PIPE || token->prev->type == END)
		token->type = CMD;
	else
		token->type = ARG;
}

t_token	*create_token(char **elements, t_shell *shell)
{
	t_token *head = NULL;
	t_token *current = NULL;
	t_token *new_token;
	int i = 0;
	while (elements[i])
	{
		new_token = malloc(sizeof(t_token));
        if (!new_token)
	{
		t_token *tmp;
		while (head)
		{
			tmp = head;
			head = head->next;
			free(tmp->str);
			free(tmp);
		}
		return (NULL);
	}
		new_token->str = expand_variables(elements[i], shell);
		new_token->next = NULL;
		new_token->prev = current;
		type_arg(new_token);
		if (!head)
			head = new_token;
		else
			current->next = new_token;
		current = new_token;
		i++;
	}
	return (head);
}

t_token	*tokenize(t_shell *shell)
{
	char	**elements;
	**elements = ft_split_quotes(shell->input);

	if (!elements)
		return (NULL);

t_token	*tokens = create_token(elements, shell);

    int i = 0;
    while (elements[i])
        free(elements[i++]);
    free(elements);

    return tokens;
}
