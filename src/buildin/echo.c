/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	has_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	if (!args)
		return (0);
	newline = 1;
	i = 1;
	if (args[1] && has_n_flag(args[1]))
	{
		newline = 0;
		i = 2;
	}
	while (args[i])
	{
		if (write(STDOUT_FILENO, args[i], ft_strlen(args[i])) == -1)
			return (1);
		if (args[i + 1])
			if (write(STDOUT_FILENO, " ", 1) == -1)
				return (1);
		i++;
	}
	if (newline)
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (1);
	return (0);
}
