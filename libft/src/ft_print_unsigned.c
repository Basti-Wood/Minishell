/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 11:43:04 by sholz             #+#    #+#             */
/*   Updated: 2024/12/07 11:43:06 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_print_unsigned(unsigned int n)
{
	char	c;
	int		len;

	len = 0;
	if (n < 10)
	{
		c = n + '0';
		ft_print_char(c);
		return (1);
	}
	else
	{
		len += ft_print_unsigned(n / 10);
		len += ft_print_unsigned(n % 10);
	}
	return (len);
}
