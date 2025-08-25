/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_format_specifier_helper.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:18:12 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 18:18:29 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
