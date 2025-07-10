/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:00:18 by sholz             #+#    #+#             */
/*   Updated: 2024/10/28 11:00:19 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int ch)
{
	if (ch >= 'A' && ch <= 'Z')
	{
		ch += 32;
	}
	return (ch);
}
/*

#include <stdio.h>
int main() {
    char lowercase = 'b';
    char uppercase = ft_toupper(lowercase);
    printf("Original: %c, Uppercase: %c\n", lowercase, uppercase);

    return 0;
}
*/
