/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:10:36 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_redirect_operator(const char *s, int i)
{
	if (s[i] == '<' || s[i] == '>')
	{
		if (s[i + 1] && (s[i] == s[i + 1]))
			return (2);
		return (1);
	}
	return (0);
}

static int	process_character(const char *s, int *i, char *result, int *res_len)
{
	int	token_started;

	token_started = 0;
	if (handle_quote_extraction(s, i, result, res_len))
		token_started = 1;
	else if (is_operator(s[*i]))
	{
		if (*res_len > 0)
			return (-1);
		handle_operator_extraction(s, i, result, res_len);
		return (-1);
	}
	else if (ft_isspace(s[*i]))
		return (-1);
	else
	{
		token_started = 1;
		result[(*res_len)++] = s[(*i)++];
	}
	return (token_started);
}

static int	should_break(const char *s, int i)
{
	return (!s[i] || ft_isspace(s[i]));
}

static char	*finalize_token(char *result, int res_len)
{
	char	*final;

	result[res_len] = '\0';
	final = ft_strdup(result);
	free(result);
	return (final);
}

char	*extract_token(const char *s, int *pos)
{
	int		i;
	char	*result;
	int		res_len;
	int		process_result;
	int		redirect_len;
	int		in_quotes;
	char	quote_char;

	i = *pos;
	result = malloc(2048);
	if (!result)
		return (NULL);
	res_len = 0;
	in_quotes = 0;
	quote_char = 0;
	i = skip_spaces(s, i);
	redirect_len = is_redirect_operator(s, i);
	if (redirect_len)
	{
		result[res_len++] = s[i++];
		if (redirect_len == 2)
			result[res_len++] = s[i++];
		*pos = i;
		return (finalize_token(result, res_len));
	}
	while (s[i])
	{
		if (is_quote(s[i]) && !in_quotes)
		{
			quote_char = s[i];
			in_quotes = 1;
		}
		else if (in_quotes && s[i] == quote_char)
		{
			in_quotes = 0;
			quote_char = 0;
		}
		if (!in_quotes && is_operator(s[i]) && res_len > 0)
			break;
		process_result = process_character(s, &i, result, &res_len);
		if (process_result == -1)
			break;
		if (should_break(s, i) && !in_quotes)
			break;
	}
	*pos = i;
	return (finalize_token(result, res_len));
}
