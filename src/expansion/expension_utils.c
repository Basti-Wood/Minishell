/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expension_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:39:52 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 17:40:05 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*remove_quote_markers(const char *str)
{
	char	*result;
	char	*dst;
	char	*src;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	src = (char *)str;
	dst = result;
	while (*src)
	{
		if (*src > '\004' || *src < '\001')
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
	return (result);
}
