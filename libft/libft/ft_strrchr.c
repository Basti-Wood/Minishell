/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:16:15 by sholz             #+#    #+#             */
/*   Updated: 2024/10/28 11:16:17 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int	i;
	int	last;

	i = 0;
	last = -1;
	while (str[i])
	{
		if (str[i] == (char)c)
		{
			last = i;
		}
		i++;
	}
	if (last != -1)
	{
		return ((char *)&str[last]);
	}
	else if (c == '\0')
	{
		return ((char *)&str[i]);
	}
	return (NULL);
}
/*


#include <stdio.h>
#include <string.h>

int main() {
    const char *text = "Hello, world!";
    char ch = 'o';
    
    // Find the last occurrence of 'o' in the string
    char *result = ft_strrchr(text, ch);

    if (result != NULL) {
        printf("The last of '%c' is at position: %ld\n", ch, result - text);
    } else {
        printf("Character '%c' not found in the string.\n", ch);
    }

    return 0;
}
*/
