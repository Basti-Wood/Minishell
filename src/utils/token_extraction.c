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
	int		redirect_len;

	i = *pos;
	result = malloc(2048);
	if (!result)
		return (NULL);
	res_len = 0;
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
		// Check for quotes before processing
		if (is_quote(s[i]))
		{
			if (handle_quote_extraction(s, &i, result, &res_len))
			{
				// Quote was processed, continue to next character
				continue;
			}
		}
		// Check for operators that should end the current token
		if (is_operator(s[i]) && res_len > 0)
			break;
		// Check for whitespace that should end the token
		if (ft_isspace(s[i]))
			break;
		// Regular character - add it to the token
		result[res_len++] = s[i++];
	}
	*pos = i;
	return (finalize_token(result, res_len));
}
