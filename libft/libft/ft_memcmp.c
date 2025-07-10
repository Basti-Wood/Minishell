/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:27:37 by sholz             #+#    #+#             */
/*   Updated: 2024/10/28 12:27:38 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *ptr1, const void *ptr2, size_t num)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (unsigned char *)ptr1;
	str2 = (unsigned char *)ptr2;
	while (i < num && str1[i] == str2[i])
	{
		i++;
	}
	if (i == num)
	{
		return (0);
	}
	else
	{
		if (str1[i] - str2[i] > 0)
			return (1);
		else
			return (-1);
	}
}
/*

#include <stdio.h>
#include <string.h>

int main() {
    char buffer1[] = "ABC";
    char buffer2[] = "ABD";

    int result = ft_memcmp("t\200", "t\0", 2);

    if (result < 0) {
        printf("%i", ft_memcmp("t\200", "t\0", 2));
    } else if (result > 0) {
        printf("%i", ft_memcmp("t\200", "t\0", 2));
    } else {
        printf("%i", ft_memcmp("t\200", "t\0", 2));
    }

    return 0;
}
*/
