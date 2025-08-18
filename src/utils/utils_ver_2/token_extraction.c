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

static int	handle_operator_extraction(const char *s, int *i, char *result,
		int *res_len)
{
	if (is_operator(s[*i]))
	{
		if ((s[*i] == '>' && s[*i + 1] == '>') || (s[*i] == '<'
				&& s[*i + 1] == '<'))
		{
			result[(*res_len)++] = s[(*i)++];
			result[(*res_len)++] = s[(*i)++];
		}
		else
			result[(*res_len)++] = s[(*i)++];
		return (1);
	}
	return (0);
}

char	*extract_token(const char *s, int *pos)
{
	int		i;
	char	*result;
	int		res_len;
	int		token_started;
	char	*final;

	i = *pos;
	result = malloc(2048);
	res_len = 0;
	token_started = 0;
	while (s[i])
	{
		if (handle_quote_extraction(s, &i, result, &res_len))
			token_started = 1;
		else if (handle_operator_extraction(s, &i, result, &res_len))
		{
			if (!token_started)
				break ;
			else
				break ;
		}
		else if (ft_isspace(s[i]))
			break ;
		else
		{
			token_started = 1;
			result[res_len++] = s[i++];
		}
		if (!s[i] || (ft_isspace(s[i]) || is_operator(s[i])))
			break ;
	}
	result[res_len] = '\0';
	*pos = i;
	final = ft_strdup(result);
	free(result);
	return (final);
}
