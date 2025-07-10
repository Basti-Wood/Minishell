/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 12:45:25 by sholz             #+#    #+#             */
/*   Updated: 2024/12/07 12:45:28 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_print_hex(unsigned int num, const char form)
{
	int	len;

	len = 0;
	if (num >= 16)
	{
		len += ft_print_hex(num / 16, form);
		len += ft_print_hex(num % 16, form);
	}
	else
	{
		if (num <= 9)
			len += ft_print_char((num + '0'));
		else
		{
			if (form == 'x')
				len += ft_print_char((num - 10 + 'a'));
			if (form == 'X')
				len += ft_print_char((num - 10 + 'A'));
		}
	}
	return (len);
}
