/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:05:56 by sholz             #+#    #+#             */
/*   Updated: 2024/10/28 12:05:57 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;
	char	*str;
	char	ch;

	i = 0;
	str = (char *)s;
	ch = (char)c;
	while (i < n)
	{
		if (str[i] == ch)
		{
			return ((void *)&str[i]);
		}
		i++;
	}
	return (NULL);
}

/*
#include <stdio.h>
#include <string.h>


int main() {
    const char *str = "Hello, world!";
    char ch = 'o';
    size_t n = strlen(str);
    
    char *result = (char *)ft_memchr(str, ch, n);
    
    if (result) {
        printf("Found '%c' at position: %ld\n", ch, result - str);
    } else {
        printf("Character '%c' not found.\n", ch);
    }

    return 0;
}

*/
