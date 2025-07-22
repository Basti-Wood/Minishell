#include "../include/minishell.h"

static t_env_node	*create_env_node(char *env_str)
{
	t_env_node	*new_node;
	char		*equal_sign;
	size_t		key_len;

	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign)
		return (NULL);
	new_node = malloc(sizeof(t_env_node));
	if (!new_node)
		return (NULL);
	key_len = equal_sign - env_str;
	new_node->key = ft_strndup(env_str, key_len);
	new_node->value = ft_strdup(equal_sign + 1);
	new_node->next = NULL;
	return (new_node);
}

static t_env_node	*init_env(char **env)
{
	t_env_node	*head;
	t_env_node	*current;
	t_env_node	*new_node;
	int			i;

	head = NULL;
	current = NULL;
	i = 0;
	while (env[i] != NULL)
	{
		new_node = create_env_node(env[i]);
		if (new_node)
		{
			if (!head)
				head = new_node;
			else
				current->next = new_node;
			current = new_node;
		}
		i++;
	}
	return (head);
}

static int	env_size(t_env_node *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

t_env_list	init_env_list(char **env)
{
	t_env_list	ret;
	t_env_node	*envi;

	envi = init_env(env);
	ret.head = envi;
	ret.size = env_size(envi);
	return (ret);
}
