/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sholz <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:57:53 by sholz             #+#    #+#             */
/*   Updated: 2024/12/07 10:57:58 by sholz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include "libft.h"

int	ft_print_char(int c);
int	ft_printf(const char *str, ...);
int	ft_print_hex(unsigned int num, const char form);
int	ft_print_nbr(int n);
int	ft_print_unsigned(unsigned int n);
int	ft_print_str(char *str);
int	ft_print_ptr(unsigned long long ptr);

#endif
