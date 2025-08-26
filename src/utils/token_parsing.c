/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:22:49 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/18 17:23:17 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	skip_spaces(const char *s, int i)
{
	while (s[i] && ft_isspace(s[i]))
		i++;
	return (i);
}

static int	handle_quote_in_count(const char *s, int *i, char *quote)
{
	if (is_quote(s[*i]) && !*quote)
	{
		*quote = s[(*i)++];
		while (s[*i] && s[*i] != *quote)
			(*i)++;
		if (s[*i] == *quote)
			(*i)++;
		*quote = 0;
		return (1);
	}
	return (0);
}

static int	handle_operator_in_count(const char *s, int *i, int *count,
		int *in_token)
{
	if (is_operator(s[*i]))
	{
		if (!*in_token || *i == skip_spaces(s, 0))
			(*count)++;
		if ((s[*i] == '>' && s[*i + 1] == '>') || (s[*i] == '<'
				&& s[*i + 1] == '<'))
			(*i)++;
		(*i)++;
		*in_token = 0;
		return (1);
	}
	return (0);
}

static int	handle_quote_in_count(const char *s, int *i, char *quote)
{
	if (is_quote(s[*i]) && !*quote)
	{
		*quote = s[(*i)++];
		while (s[*i] && s[*i] != *quote)
			(*i)++;
		if (s[*i] == *quote)
			(*i)++;
		*quote = 0;
		return (1);
	}
	return (0);
}

static int	handle_operator_in_count(const char *s, int *i, int *count,
		int *in_token)
{
	if (is_operator(s[*i]))
	{
		if (!*in_token || *i == skip_spaces(s, 0))
			(*count)++;
		if ((s[*i] == '>' && s[*i + 1] == '>') || (s[*i] == '<'
				&& s[*i + 1] == '<'))
			(*i)++;
		(*i)++;
		*in_token = 0;
		return (1);
	}
	return (0);
}
