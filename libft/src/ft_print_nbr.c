/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_nbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 11:25:28 by sholz             #+#    #+#             */
/*   Updated: 2024/12/07 11:25:30 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_nbr(int n)
{
	int		len;

	len = 0;
	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
	if (n < 0)
	{
		len += ft_print_char('-');
		n = -n;
	}
	if (n < 10)
	{
		len += ft_print_char(n + '0');
		return (len);
	}
	else
	{
		len += ft_print_nbr(n / 10);
		len += ft_print_nbr(n % 10);
	}
	return (len);
}
