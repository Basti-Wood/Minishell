/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parsing_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:10:36 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static int	handle_regular_chars(const char *s, int *i)
{
	while (s[*i] && !ft_isspace(s[*i]) && !is_operator(s[*i])
		&& !is_quote(s[*i]))
		(*i)++;
	return (1);
}

static int	should_continue_token(const char *s, int i)
{
	return (s[i] && (is_quote(s[i]) || (!ft_isspace(s[i])
				&& !is_operator(s[i]))));
}

static int	handle_token_content(const char *s, int *i, char *quote)
{
	if (handle_quote_in_count(s, i, quote))
		return (1);
	handle_regular_chars(s, i);
	if (should_continue_token(s, *i))
		return (1);
	return (0);
}

static int	process_token_step(const char *s, int *i, int *count, int *in_token)
{
	char	quote;

	quote = 0;
	if (handle_operator_in_count(s, i, count, in_token))
		return (1);
	if (handle_token_content(s, i, &quote))
		return (1);
	*in_token = 0;
	return (0);
}

int	count_tokens(const char *s)
{
	int		count;
	int		i;
	int		in_token;

	count = 0;
	i = 0;
	in_token = 0;
	while (s[i])
	{
		i = skip_spaces(s, i);
		if (!s[i])
			break ;
		if (!in_token)
		{
			count++;
			in_token = 1;
		}
		if (process_token_step(s, &i, &count, &in_token))
			continue ;
	}
	return (count);
}
