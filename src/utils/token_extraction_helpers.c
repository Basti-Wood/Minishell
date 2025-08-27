/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:24:43 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/18 18:09:13 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_quote_markers(char quote, char *result, int *res_len, int is_end)
{
	if (quote == '\'')
	{
		if (is_end)
			result[(*res_len)++] = '\003';
		else
			result[(*res_len)++] = '\001';
	}
	else
	{
		if (is_end)
			result[(*res_len)++] = '\004';
		else
			result[(*res_len)++] = '\002';
	}
}

int	handle_quote_extraction(const char *s, int *i, char *result, int *res_len)
{
	char	quote;

	if (is_quote(s[*i]))
	{
		quote = s[(*i)++];
		add_quote_markers(quote, result, res_len, 0);
		while (s[*i] && s[*i] != quote)
			result[(*res_len)++] = s[(*i)++];
		if (s[*i] == quote)
		{
			(*i)++;
			add_quote_markers(quote, result, res_len, 1);
		}
		return (1);
	}
	return (0);
}

int	handle_operator_extraction(const char *s, int *i, char *result,
		int *res_len)
{
	if (is_operator(s[*i]))
	{
		result[(*res_len)++] = s[(*i)++];
		if ((s[*i - 1] == '>' && s[*i] == '>') || (s[*i - 1] == '<'
				&& s[*i] == '<'))
		{
			result[(*res_len)++] = s[(*i)++];
		}
		return (1);
	}
	return (0);
}
