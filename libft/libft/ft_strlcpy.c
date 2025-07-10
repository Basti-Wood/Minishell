/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:03:13 by sholz             #+#    #+#             */
/*   Updated: 2024/10/23 16:03:14 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	if (dstsize == 0)
		return (ft_strlen(src));
	i = 0;
	while (i < (dstsize - 1) && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}
/*


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    // Define a source string
    const char *src = "Hello, World!";
    
    // Create a destination buffer with a specific size
    char dest[6];

    // Copy the string using my_strlcpy and ensure null-termination
    size_t result = ft_strlcpy(dest, src, sizeof(dest));

    // Print the copied string and the result
    printf("Copied string: %s\n", dest);
    printf("Total length of source string: %zu\n", result);

    // Check if the string was truncated
    if (result >= sizeof(dest)) {
        printf("The string was truncated!\n");
    } else {
        printf("The string fits perfectly.\n");
    }

    return 0;
}
*/
