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

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*str;

	str = (unsigned char *)s;
	while (n--)
	{
		*str++ = 0;
	}
}

/*


#include <stdio.h>
#include <strings.h>  // for bzero

int main() {
    // Define a character array (buffer) with some initial data
    char buffer[10] = "hello";

    // Print the buffer before bzero
    printf("Buffer before bzero: '%s'\n", buffer);

    // Use bzero to zero out the memory of the buffer
    bzero(buffer, sizeof(buffer));

    // Print the buffer after bzero
    printf("Buffer after bzero: '%s'\n", buffer);

    return 0;
}
*/
