/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:55:09 by sholz             #+#    #+#             */
/*   Updated: 2024/10/28 10:55:12 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_toupper(int ch)
{
	if (ch >= 'a' && ch <= 'z')
	{
		ch -= 32;
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
hey you i love c (no i dont')
*/
