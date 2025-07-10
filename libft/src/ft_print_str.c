/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 11:21:06 by sholz             #+#    #+#             */
/*   Updated: 2024/12/07 11:21:08 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_print_str(char *str)
{
	int	i;

	if (str == NULL)
	{
		ft_print_str("(null)");
		return (6);
	}
	i = 0;
	while (str[i])
	{
		ft_print_char(str[i]);
		i++;
	}
	return (i);
}
