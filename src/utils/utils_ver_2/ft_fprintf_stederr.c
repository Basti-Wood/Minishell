/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf_stederr.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/18 17:08:56 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdarg.h>

int	ft_fprintf_stderr(const char *format, ...)
{
	va_list	args;
	int		len;
	int		i;

	if (!format)
		return (-1);
	va_start(args, format);
	len = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			handle_format_specifier(args, format[i], STDERR_FILENO, &len);
		}
		else if (format[i] == '%')
			ft_putchar_fd_len('%', STDERR_FILENO, &len);
		else
			ft_putchar_fd_len(format[i], STDERR_FILENO, &len);
		i++;
	}
	va_end(args);
	return (len);
}
