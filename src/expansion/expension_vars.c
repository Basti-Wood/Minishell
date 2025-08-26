/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expension_vars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:40:23 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 17:40:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_var_name(char **src)
{
	char	*start;
	char	*result;
	int		len;

	start = *src;
	len = 0;
	while (**src && (ft_isalnum(**src) || **src == '_'))
	{
		(*src)++;
		len++;
	}
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, start, len);
	result[len] = '\0';
	return (result);
}

char	*copy_string_to_dst(char *dst, const char *src)
{
	while (*src)
		*dst++ = *src++;
	return (dst);
}
