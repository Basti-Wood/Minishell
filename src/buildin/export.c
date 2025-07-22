#include "../include/minishell.h"

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_export_list(t_env_list *env_list)
{
	t_env_node	*current;

	current = env_list->head;
	while (current)
	{
		printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
}

static int	handle_export_with_value(char *arg, t_env_list *env_list)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	*equal_sign = '\0';
	key = arg;
	value = equal_sign + 1;
	if (!is_valid_identifier(key))
	{
		ft_fprintf_stderr("minishell: export: `%s': not a valid identifier\n",
			key);
		*equal_sign = '=';
		return (1);
	}
	set_env_value(env_list, key, value);
	*equal_sign = '=';
	return (0);
}

static int	handle_export_without_value(char *key, t_env_list *env_list)
{
	if (!is_valid_identifier(key))
	{
		ft_fprintf_stderr("minishell: export: `%s': not a valid identifier\n",
			key);
		return (1);
	}
	if (!get_env_value(env_list, key))
		set_env_value(env_list, key, "");
	return (0);
}

int	builtin_export(char **args, t_env_list *env_list)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	if (!args[1])
	{
		print_export_list(env_list);
		return (0);
	}
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			exit_status |= handle_export_with_value(args[i], env_list);
		else
			exit_status |= handle_export_without_value(args[i], env_list);
		i++;
	}
	return (exit_status);
}
