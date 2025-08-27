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
	return (1);
}

static int	handle_exit_args(char **args, char **clean_arg, int *exit_code)
{
	*clean_arg = remove_quote_markers(args[1]);
	if (!is_valid_number(*clean_arg))
	{
		ft_fprintf_stderr
			("minishell: exit: %s: numeric argument required\n", args[1]);
		free(*clean_arg);
		exit(255);
	}
	if (args[2])
	{
		ft_fprintf_stderr("minishell: exit: too many arguments\n");
		free(*clean_arg);
		return (1);
	}
	*exit_code = ft_atoi(*clean_arg);
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
		exit_code = shell->exit_status;
		exit(exit_code);
	}
	result = handle_exit_args(args, &clean_arg, &exit_code);
	if (result != 0)
		return (result);
	exit(exit_code);
}
