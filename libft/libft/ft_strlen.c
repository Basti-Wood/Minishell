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

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}
/*



#include <stdio.h>
#include <string.h>

int main() {
    // Example string
    char str1[] = "Hello, World!";
    char str2[] = "";  // Empty string

    // Using strlen to calculate the length of the strings
    size_t len1 = ft_strlen(str1);  // Should return 13 
    size_t len2 = ft_strlen(str2);  // Should return 0
	size_t len3 = ft_strlen();
    // Displaying the lengths
    printf("Length of '%s' is: %zu\n", str1, len1);
    printf("Length of an empty string is: %zu\n", len2);
    printf("Length of an empty string is: %zu\n", len3);

    return 0;
}
*/
