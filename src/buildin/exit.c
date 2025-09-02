/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:02:55 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 19:53:59 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <limits.h>

static int	is_number_in_range(const char *str)
{
	long long	num;
	int			i;
	int			sign;

	i = 0;
	sign = 1;
	num = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] && ft_isdigit(str[i]))
	{
		num = num * 10 + (str[i] - '0');
		if ((sign == 1 && num > INT_MAX)
			|| (sign == -1 && num > ((long long)INT_MAX + 1)))
			return (0);
		i++;
	}
	return (1);
}

static int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i] || !ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (is_number_in_range(str));
}

static int	handle_exit_args(char **args, char **clean_arg,
			int *exit_code, t_shell *shell)
{
	*clean_arg = remove_quote_markers(args[1]);
	if (!is_valid_number(*clean_arg))
	{
		ft_fprintf_stderr("minishell: exit: %s: numeric argument required\n",
			args[1]);
		free(*clean_arg);
		shell->exit_status = 2;
		return (2);
	}
	if (args[2])
	{
		ft_fprintf_stderr("minishell: exit: too many arguments\n");
		free(*clean_arg);
		shell->exit_status = 1;
		return (1);
	}
	*exit_code = ft_atoi(*clean_arg) % 256;
	if (*exit_code < 0)
		*exit_code += 256;
	free(*clean_arg);
	return (0);
}

int	builtin_exit(char **args, t_shell *shell)
{
	char	*clean_arg;
	int		exit_code;
	int		result;

	printf("exit\n");
	if (!args[1])
	{
		exit_code = shell->exit_status % 256;
		if (exit_code < 0)
			exit_code += 256;
		shell->exit_status = exit_code;
		shell->should_exit = 1;
		return (exit_code);
	}
	result = handle_exit_args(args, &clean_arg, &exit_code, shell);
	if (result != 0)
		return (result);
	shell->exit_status = exit_code;
	shell->should_exit = 1;
	return (exit_code);
}
