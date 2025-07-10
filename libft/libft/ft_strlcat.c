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

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = 0;
	src_len = 0;
	if (!dst && size == 0)
		return (ft_strlen(src));
	while (dst[dst_len] && dst_len < size)
		dst_len++;
	while (src[src_len])
		src_len++;
	if (dst_len == size)
		return (size + src_len);
	i = 0;
	while (src[i] && (dst_len + i + 1) < size)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	if (dst_len + i < size)
		dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}

/*
#include <stdio.h>
#include <string.h>

int main() {
    char dest[20] = "Hello, ";  // Destination string
    const char *src = "World!";
    size_t maxSize = sizeof(dest) - 1; // Calculate space for concatenation

    // Concatenating src to dest with size limit
    ft_strlcat(dest, src, maxSize);

    // Output the result
    printf("Concatenated String: %s\n", dest);

    return 0;
}
*/
