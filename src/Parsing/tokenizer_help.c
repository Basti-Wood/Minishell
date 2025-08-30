#include "../../include/minishell.h"

static int	check_for_quotes(char *str)
{
	while (*str)
	{
		if (*str == '\001' || *str == '\002')
			return (1);
		if (*str == '\003' || *str == '\004')
			return (1);
		str++;
	}
	return (0);
}

static t_token	*create_quoted_token(char *expanded, t_token *prev)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = expanded;
	new_token->type = EMPTY;
	new_token->next = NULL;
	new_token->prev = prev;
	return (new_token);
}

static t_token	*split_expanded_token(char *expanded, t_token *prev)
{
	t_token	*head;
	char	**words;
	int		i;

	if (check_for_quotes(expanded) || ft_strlen(expanded) == 0)
		return (create_quoted_token(expanded, prev));
	words = ft_split(expanded, ' ');
	if (!words || !words[0])
	{
		if (words)
			free_argv(words);
		free(expanded);
		return (NULL);
	}
	head = NULL;
	i = 0;
	while (words[i])
	{
		append_token(&head, create_new_token(ft_strdup(words[i]), EMPTY));
		i++;
	}
	free_argv(words);
	free(expanded);
	return (head);
}

static t_token	*create_single_token(char *element,
				t_shell *shell, t_token *prev)
{
	char	*expanded;

	if (!element)
		return (NULL);
	expanded = expand_variables(element, shell);
	if (!expanded)
		return (NULL);
	return (split_expanded_token(expanded, prev));
}

t_token	*create_token(char **elements, t_shell *shell)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_tokens;
	t_token	*last;
	int		i;

	if (!elements || !shell)
		return (NULL);
	head = NULL;
	current = NULL;
	i = 0;
	while (elements[i])
	{
		new_tokens = create_single_token(elements[i], shell, current);
		if (!new_tokens)
		{
			free_tokens(head);
			return (NULL);
		}
		last = new_tokens;
		while (last->next)
			last = last->next;
		if (current)
			current->next = new_tokens;
		else
			head = new_tokens;
		current = last;
		i++;
	}
	return (head);
}