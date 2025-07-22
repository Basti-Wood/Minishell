#include "../include/minishell.h"

static int	skip_quote_markers(const char *s, int i)
{
	while (s[i] && (s[i] == '\001' || s[i] == '\002' || s[i] == '\003'
			|| s[i] == '\004'))
		i++;
	return (i);
}

static int	is_valid_digit_part(const char *s, int i)
{
	while (s[i])
	{
		if (!ft_isnum(s[i]) && s[i] != '\001' && s[i] != '\002'
			&& s[i] != '\003' && s[i] != '\004')
			return (0);
		i++;
	}
	return (1);
}

static int	is_numeric_str(const char *s)
{
	int	i;

	i = 0;
	i = skip_quote_markers(s, i);
	while (s[i] && ft_isspace(s[i]))
		i++;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i] || !ft_isnum(s[i]))
		return (0);
	return (is_valid_digit_part(s, i));
}

static int	get_exit_code(char *arg)
{
	char	*clean_arg;
	int		exit_code;

	clean_arg = remove_quote_markers(arg);
	exit_code = ft_atoi(clean_arg);
	free(clean_arg);
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code += 256;
	return (exit_code);
}

int	builtin_exit(char **args)
{
	int	exit_code;

	exit_code = 0;
	printf("exit\n");
	if (args[1])
	{
		if (!is_numeric_str(args[1]))
		{
			ft_fprintf_stderr(
				"minishell: exit: %s: numeric argument required\n",
				args[1]);
			exit(2);
		}
		if (args[2])
		{
			ft_fprintf_stderr("minishell: exit: too many arguments\n");
			return (1);
		}
		exit_code = get_exit_code(args[1]);
	}
	exit(exit_code);
}
