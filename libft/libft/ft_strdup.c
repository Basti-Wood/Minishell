/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:25:39 by sholz             #+#    #+#             */
/*   Updated: 2024/10/30 17:25:41 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	char	*dup;
	size_t	i;
	size_t	len;

	len = 0;
	while (str[len])
	{
		len += 1;
	}
	i = 0;
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (!dup)
	{
		return (NULL);
	}
	while (str[i])
	{
		dup[i] = str[i];
		i += 1;
	}
	dup[i] = '\0';
	return (dup);
}
/*


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    // Original string
    char original[] = "Hello, World!";
    
    // Duplicate the string
    char *duplicate = ft_strdup(original);
    
    // Check if strdup was successful
    if (duplicate == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Print both strings
    printf("Original string: %s\n", original);
    printf("Duplicated string: %s\n", duplicate);

    // Modify the duplicated string
    duplicate[0] = 'h';
    printf("Modified duplicated string: %s\n", duplicate);

    // The original string is unchanged
    printf("Original string after modification: %s\n", original);

    // Free the duplicated string's memory
    free(duplicate);

    return 0;
}
*/
