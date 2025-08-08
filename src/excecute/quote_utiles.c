/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utiles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:55:13 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 16:55:27 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_quote_marker(char c)
{
	return (c == '\001' || c == '\002' || c == '\003' || c == '\004');
}

static void	copy_char_if_valid(const char *str, char *result, int *i, int *j)
{
	if (is_quote_marker(str[*i]))
		(*i)++;
	else
		result[(*j)++] = str[(*i)++];
}

char	*remove_quote_markers(const char *str)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
		copy_char_if_valid(str, result, &i, &j);
	result[j] = '\0';
	return (result);
}
