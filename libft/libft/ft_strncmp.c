/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:25:50 by sholz             #+#    #+#             */
/*   Updated: 2024/10/28 11:25:51 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;

	if (!n)
	{
		return (0);
	}
	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	while (*str1 && (*str1 == *str2) && --n)
	{
		if (*str1 != *str2)
		{
			return (*str1 - *str2);
		}
		str1 += 1;
		str2 += 1;
	}
	return (*str1 - *str2);
}
/*


#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "hello";
    char str2[] = "hello, world";
    
    // Compare first 5 characters
    if (ft_strncmp(str1, str2, 5) == 0) {
        printf("The first 5 characters are the same.\n");
    } else {
        printf("The first 5 characters are different.\n");
    }

    return 0;
}
*/
