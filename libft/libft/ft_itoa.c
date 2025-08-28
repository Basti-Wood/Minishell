/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HOMEft_itoa.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:05:11 by sholz             #+#    #+#             */
/*   Updated: 2024/11/04 18:05:14 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_nbrlen(int nbr)
{
	size_t	len;

	len = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
		len += 1;
	while (nbr != 0)
	{
		nbr = nbr / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	size_t	i;
	size_t	n_size;
	char	*str;
	long	ln;

	i = 0;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	ln = n;
	if (ln < 0)
		ln = -ln;
	n_size = ft_nbrlen(n);
	str = (char *)malloc(sizeof(char) * (n_size + 1));
	if (str == 0)
		return (0);
	str[n_size] = '\0';
	if (n < 0)
	{
		str[0] = '-';
		i = 1;
	}
	while (n_size > i)
	{
		str[n_size - 1] = (ln % 10) + '0';
		ln /= 10;
		n_size--;
	}
	return (str);
}
