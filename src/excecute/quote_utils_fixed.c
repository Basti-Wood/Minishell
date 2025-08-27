/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils_fixed.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Fixed version for memory issues            #+#    #+#             */
/*   Updated: Fixed version for memory issues            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*remove_quote_markers(const char *str)
{
	char	*result;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	
	// ALWAYS allocate new memory to avoid double-free issues
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	
	i = 0;
	j = 0;
	while (str[i])
	{
		// Skip quote markers but copy everything else
		if (str[i] != SINGLE_QUOTE_START && 
		    str[i] != SINGLE_QUOTE_END &&
		    str[i] != DOUBLE_QUOTE_START && 
		    str[i] != DOUBLE_QUOTE_END)
		{
			result[j++] = str[i];
		}
		i++;
	}
	result[j] = '\0';
	
	return (result);
}
