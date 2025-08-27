/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_format_specifier.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_string_specifier(va_list args, int fd, int *len)
{
	char	*str;

	str = va_arg(args, char *);
	if (!str)
		ft_putstr_fd_len("(null)", fd, len);
	else
		ft_putstr_fd_len(str, fd, len);
}

static void	handle_char_specifier(va_list args, int fd, int *len)
{
	char	c;

	c = (char)va_arg(args, int);
	ft_putchar_fd_len(c, fd, len);
}

static void	handle_number_specifier(va_list args, int fd, int *len)
{
	int	num;

	num = va_arg(args, int);
	ft_putnbr_fd_len(num, fd, len);
}

static void	handle_percent_specifier(int fd, int *len)
{
	ft_putchar_fd_len('%', fd, len);
}

static void	handle_unknown_specifier(char specifier, int fd, int *len)
{
	ft_putchar_fd_len('%', fd, len);
	ft_putchar_fd_len(specifier, fd, len);
}

void	handle_format_specifier(va_list args, char specifier, int fd, int *len)
{
	if (specifier == 's')
		handle_string_specifier(args, fd, len);
	else if (specifier == 'c')
		handle_char_specifier(args, fd, len);
	else if (specifier == 'd' || specifier == 'i')
		handle_number_specifier(args, fd, len);
	else if (specifier == '%')
		handle_percent_specifier(fd, len);
	else
		handle_unknown_specifier(specifier, fd, len);
}
