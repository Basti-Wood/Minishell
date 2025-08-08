/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expension_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:41:14 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 17:41:22 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expand_variables(char *str, t_shell *shell)
{
	char	*result;
	char	*src;
	char	*dst;
	int		in_single_quotes;

	result = malloc(ft_strlen(str) * 3 + 1);
	if (!result)
		return (NULL);
	src = str;
	dst = result;
	in_single_quotes = 0;
	while (*src)
	{
		if (*src == '\001')
			in_single_quotes = 1;
		else if (*src == '\003')
			in_single_quotes = 0;
		else if (*src == '$' && !in_single_quotes)
			dst = expand_dollar(&src, dst, shell);
		else if (*src != '\002' && *src != '\004')
			*dst++ = *src;
		if (*src)
			src++;
	}
	*dst = '\0';
	return (result);
}
