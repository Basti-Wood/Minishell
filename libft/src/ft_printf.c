/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:58:25 by sholz             #+#    #+#             */
/*   Updated: 2024/12/07 10:58:27 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static	int	pft_form(va_list args, const char form)
{
	int	len;

	len = 0;
	if (form == 'c')
		len += ft_print_char(va_arg(args, int));
	else if (form == 's')
		len += ft_print_str(va_arg(args, char *));
	else if (form == 'p')
		len += ft_print_ptr(va_arg(args, unsigned long long));
	else if (form == 'd' || form == 'i')
		len += ft_print_nbr(va_arg(args, int));
	else if (form == 'u')
		len += ft_print_unsigned(va_arg(args, unsigned int));
	else if (form == 'x' || form == 'X')
		len += ft_print_hex(va_arg(args, unsigned int), form);
	else if (form == '%')
		len += ft_print_char('%');
	return (len);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	va_list	args;
	int		len;

	i = 0;
	len = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			len += pft_form(args, str[i + 1]);
			i++;
		}
		else
			len += ft_print_char(str[i]);
		i++;
	}
	va_end(args);
	return (len);
}
