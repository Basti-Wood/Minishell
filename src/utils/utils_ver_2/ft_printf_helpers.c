/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/18 17:03:37 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_putstr_fd_len(char *s, int fd, int *len)
{
	int	str_len;

	if (!s)
		return ;
	str_len = ft_strlen(s);
	write(fd, s, str_len);
	*len += str_len;
}

void	ft_putchar_fd_len(char c, int fd, int *len)
{
	write(fd, &c, 1);
	(*len)++;
}

void	ft_putnbr_fd_len(int n, int fd, int *len)
{
	char	*num_str;

	num_str = ft_itoa(n);
	if (num_str)
	{
		ft_putstr_fd_len(num_str, fd, len);
		free(num_str);
	}
}
