#include "../../include/minishell.h"

static int	has_quote_markers(char *str)
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

static t_token_type	get_operator_type(char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (PIPE);
	if (ft_strcmp(str, "<") == 0)
		return (INPUT);
	if (ft_strcmp(str, ">") == 0)
		return (TRUNC);
	if (ft_strcmp(str, ">>") == 0)
		return (APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (HEREDOC);
	return (EMPTY);
}

static t_token_type	determine_type(char *str, int is_first)
{
	t_token_type	op_type;

	op_type = get_operator_type(str);
	if (op_type != EMPTY)
		return (op_type);
	if (has_quote_markers(str) && is_first)
		return (CMD);
	if (is_first)
		return (CMD);
	return (ARG);
}

static int	should_skip_next(t_token *current)
{
	if (!current)
		return (0);
	if (current->type == INPUT || current->type == TRUNC)
		return (1);
	if (current->type == APPEND || current->type == HEREDOC)
		return (1);
	return (0);
}

t_token	*assign_token_types(t_token *tokens)
{
	t_token	*current;
	int		is_first;
	int		skip_next;

	if (!tokens)
		return (NULL);
	current = tokens;
	is_first = 1;
	skip_next = 0;
	while (current)
	{
		if (current->type == EMPTY && current->str)
		{
			if (!skip_next)
			{
				current->type = determine_type(current->str, is_first);
				if (current->type == CMD)
					is_first = 0;
				skip_next = should_skip_next(current);
			}
			else
			{
				current->type = ARG;
				skip_next = 0;
			}
		}
		if (current->type == PIPE)
			is_first = 1;
		current = current->next;
	}
	return (tokens);
}