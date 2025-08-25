/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:11:14 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/18 18:11:25 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_split_quotes(const char *input)
{
	int		count;
	char	**tokens;
	int		i;
	int		token_idx;

	if (!input)
		return (NULL);
	count = count_tokens(input);
	tokens = malloc(sizeof(char *) * (count + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	token_idx = 0;
	while (input[i] && token_idx < count)
	{
		i = skip_spaces(input, i);
		if (!input[i])
			break ;
		tokens[token_idx] = extract_token(input, &i);
		token_idx++;
	}
	tokens[token_idx] = NULL;
	return (tokens);
}
