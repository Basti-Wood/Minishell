#include "../include/minishell.h"

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
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}