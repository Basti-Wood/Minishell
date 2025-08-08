/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:03:25 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:03:26 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

static int	is_numeric_str(const char *s)
{
	int	i;

	i = 0;
	*clean_arg = remove_quote_markers(args[1]);
	while (s[i] && (s[i] == '\001' || s[i] == '\002'
			|| s[i] == '\003' || s[i] == '\004'))
		i++;
	while (s[i] && ft_isspace(s[i]))
		i++;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i] || !ft_isnum(s[i]))
		return (0);
	while (s[i])
	{
		if (!ft_isnum(s[i]) && s[i] != '\001' && s[i]
			!= '\002' && s[i] != '\003' && s[i] != '\004')
			return (0);
		i++;
		return (1);
	}
}

int	builtin_exit(char **args)
{
	int		exit_code;
	char	*clean_arg;

	exit_code = 0;
	printf("exit\n");
	if (args[1])
	{
		if (!is_numeric_str(args[1]))
		{
			ft_fprintf_stderr(
				"minishell: exit: %s:numeric argument required\n", args[1]);
			exit(2);
		}
		if (args[2])
		{
			ft_fprintf_stderr("minishell: exit: too many arguments\n");
			return (1);
		}
	}
	exit_code = ft_atoi(clean_arg);
	free(clean_arg);
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code += 256;
}
exit(exit_code);
}
