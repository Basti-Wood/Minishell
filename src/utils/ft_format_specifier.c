/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_format_specifier.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/22 14:38:01 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
