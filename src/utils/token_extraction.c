/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:10:36 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/18 18:10:51 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	process_character(const char *s, int *i, char *result, int *res_len)
{
	int	token_started;

	token_started = 0;
	if (handle_quote_extraction(s, i, result, res_len))
		token_started = 1;
	else if (handle_operator_extraction(s, i, result, res_len))
		return (-1);
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
	return (!s[i] || ft_isspace(s[i]) || is_operator(s[i]));
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
	int		token_started;
	int		process_result;

	i = *pos;
	result = malloc(2048);
	if (!result)
		return (NULL);
	res_len = 0;
	token_started = 0;
	while (s[i])
	{
		process_result = process_character(s, &i, result, &res_len);
		if (process_result == -1)
			break ;
		if (process_result == 1)
			token_started = 1;
		if (should_break(s, i))
			break ;
	}
	*pos = i;
	return (finalize_token(result, res_len));
}
