/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:25:56 by sholz             #+#    #+#             */
/*   Updated: 2024/10/23 15:26:41 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*ndst;
	char	*nsrc;
	size_t	i;

	i = 0;
	ndst = (char *)dst;
	nsrc = (char *)src;
	if (ndst > nsrc)
	{
		while (len-- > 0)
		{
			ndst[len] = nsrc[len];
		}
	}
	else
	{
		while (i < len)
		{
			ndst[i] = nsrc[i];
			i++;
		}
	}
	return (ndst);
}
/*
#include <stdio.h>
#include <string.h>

int main() {
    // Define a source string
    char str[] = "Hello, World!";
    
    // Display the original string
    printf("Original string: %s\n", str);
    
    // Use memmove to copy "World" and overwrite part of the string
    ft_memmove(str + 5, str + 7, 6); // Move "World!" index 7 to index 5

    // Modify the destination area to terminate the string properly
    str[11] = '\0'; // Terminate string after 'World'
    
    // Display the modified string
    printf("Modified string: %s\n", str);
    
    return 0;
}
*/
