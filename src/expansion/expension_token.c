/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expension_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:41:35 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 17:41:43 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
#include "../../include/minishell.h"

static char	*allocate_result_buffer(char *str)
{
	char	*result;

	result = malloc(ft_strlen(str) * 3 + 1);
	if (!result)
		return (NULL);
	return (result);
}

static void	handle_quote_markers(char c, int *in_single_quotes)
{
	if (c == '\001')
		*in_single_quotes = 1;
	else if (c == '\003')
		*in_single_quotes = 0;
}

static int	should_copy_char(char c)
{
	return (c != '\002' && c != '\004');
}

char	*expand_variables(char *str, t_shell *shell)
{
	char	*result;
	char	*src;
	char	*dst;
	int		in_single_quotes;

	result = allocate_result_buffer(str);
	if (!result)
		return (NULL);
	src = str;
	dst = result;
	in_single_quotes = 0;
	while (*src)
	{
		handle_quote_markers(*src, &in_single_quotes);
		if (*src == '$' && !in_single_quotes)
			dst = expand_dollar(&src, dst, shell);
		else if (should_copy_char(*src))
			*dst++ = *src;
		if (*src)
			src++;
	}
	*dst = '\0';
	return (result);
}*/
