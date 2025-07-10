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

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*ndst;
	char	*nsrc;

	i = 0;
	ndst = (char *)dst;
	nsrc = (char *)src;
	while (n--)
	{
		ndst[i] = nsrc[i];
		i++;
	}
	return (dst);
}
/*

#include <stdio.h>
#include <string.h>

int main() {
    // Source array
    char src[] = "Hello, memcpy!";
    
    // Destination array (big enough to hold src)
    char dest[20];

    // Using memcpy to copy the contents of src into dest
    ft_memcpy(dest, src, strlen(src) + 1);  // +1 to include the null terminator

    // Print the source and destination
    printf("Source: %s\n", src);
    printf("Destination: %s\n", dest);

    return 0;
}
*/
