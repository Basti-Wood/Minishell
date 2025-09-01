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

static int	get_operator_length(const char *s, int i)
{
	if (s[i] == '|')
		return (1);
	if (s[i] == '<' || s[i] == '>')
	{
		if (s[i + 1] && (s[i] == s[i + 1]))
			return (2);
		return (1);
	}
	return (0);
}

static char	*create_operator_token(const char *s, int *pos)
{
	char	*result;
	int		op_len;
	int		i;

	op_len = get_operator_length(s, *pos);
	if (!op_len)
		return (NULL);
	result = malloc(op_len + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < op_len)
	{
		result[i] = s[*pos + i];
		i++;
	}
	result[i] = '\0';
	*pos += op_len;
	return (result);
}

static char	*finalize_result(char *buffer, int len)
{
	char	*final;

	buffer[len] = '\0';
	final = ft_strdup(buffer);
	free(buffer);
	return (final);
}

static int	process_char(const char *s, int *i, char *result, int *res_len)
{
	if (is_quote(s[*i]))
		return (handle_quote_extraction(s, i, result, res_len));
	if (is_operator(s[*i]))
		return (0);
	if (ft_isspace(s[*i]))
		return (0);
	result[(*res_len)++] = s[(*i)++];
	return (1);
}

char	*extract_token(const char *s, int *pos)
{
	char	*result;
	char	*operator_token;
	int		res_len;
	int		i;

	i = skip_spaces(s, *pos);
	*pos = i;
	operator_token = create_operator_token(s, pos);
	if (operator_token)
		return (operator_token);
	result = malloc(2048);
	if (!result)
		return (NULL);
	res_len = 0;
	while (s[i])
	{
		if (!process_char(s, &i, result, &res_len))
			break ;
	}
	*pos = i;
	return (finalize_result(result, res_len));
}
