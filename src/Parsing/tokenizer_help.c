#include "../../include/minishell.h"

static t_token	*split_expanded_token(char *expanded, t_token *prev)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	char	**words;
	int		i;
	int		has_quotes;
	char	*p;

	head = NULL;
	current = NULL;
	has_quotes = 0;
	p = expanded;
	while (*p)
	{
		if (*p == '\001' || *p == '\002' || *p == '\003' || *p == '\004')
		{
			has_quotes = 1;
			break;
		}
		p++;
	}
	if (has_quotes || ft_strlen(expanded) == 0)
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			return (NULL);
		new_token->str = expanded;
		new_token->type = EMPTY;
		new_token->next = NULL;
		new_token->prev = prev;
		return (new_token);
	}
	words = ft_split(expanded, ' ');
	if (!words || !words[0])
	{
		if (words)
			free_argv(words);
		free(expanded);
		return (NULL);
	}
	i = 0;
	while (words[i])
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
		{
			free_argv(words);
			free_tokens(head);
			free(expanded);
			return (NULL);
		}
		new_token->str = ft_strdup(words[i]);
		if (!new_token->str)
		{
			free(new_token);
			free_argv(words);
			free_tokens(head);
			free(expanded);
			return (NULL);
		}
		new_token->type = EMPTY;
		new_token->next = NULL;
		new_token->prev = current ? current : prev;
		if (current)
			current->next = new_token;
		else
			head = new_token;
		current = new_token;
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

static int	has_quote_markers(char *str)
{
	while (str && *str)
	{
		if (*str == '\001' || *str == '\002' || *str == '\003' || *str == '\004')
			return (1);
		str++;
	}
	return (0);
}

static t_token_type	get_token_type(char *str, int is_first)
{
	if (has_quote_markers(str))
	{
		if (is_first)
			return (CMD);
		else
			return (ARG);
	}
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
