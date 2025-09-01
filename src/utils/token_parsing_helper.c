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

static void	skip_quoted_content(const char *s, int *i)
{
	char	quote;

	quote = s[(*i)++];
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i] == quote)
		(*i)++;
}

static void	skip_regular_word(const char *s, int *i)
{
	while (s[*i] && !ft_isspace(s[*i]) && !is_operator(s[*i]))
	{
		if (is_quote(s[*i]))
			skip_quoted_content(s, i);
		else
			(*i)++;
	}
}

static void	skip_operator(const char *s, int *i)
{
	if (s[*i] == '|')
		(*i)++;
	else if ((s[*i] == '>' && s[*i + 1] == '>') || (s[*i] == '<' && s[*i
				+ 1] == '<'))
		(*i) += 2;
	else
		(*i)++;
}

int	count_tokens(const char *s)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		i = skip_spaces(s, i);
		if (!s[i])
			break ;
		count++;
		if (is_operator(s[i]))
			skip_operator(s, &i);
		else
			skip_regular_word(s, &i);
	}
	return (count);
}
