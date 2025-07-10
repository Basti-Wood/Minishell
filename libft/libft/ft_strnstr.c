/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:37:56 by sholz             #+#    #+#             */
/*   Updated: 2024/10/28 12:37:57 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hay, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	char	*h;
	char	*n;

	h = (char *)hay;
	n = (char *)needle;
	if (n[0] == '\0')
		return ((char *)h);
	i = 0;
	while (h[i] != '\0' && i < len)
	{
		j = 0;
		while (h[i + j] == n[j] && (i + j) < len)
		{
			if (n[j + 1] == '\0')
				return ((char *)&h[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}
