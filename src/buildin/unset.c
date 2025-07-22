#include "../include/minishell.h"

static void	remove_env_node(t_env_list *env_list, t_env_node *node,
	t_env_node *prev)
{
	if (prev)
		prev->next = node->next;
	else
		env_list->head = node->next;
	free(node->key);
	free(node->value);
	free(node);
	env_list->size--;
}

static void	unset_env_var(char *key, t_env_list *env_list)
{
	t_env_node	*current;
	t_env_node	*prev;

	current = env_list->head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			remove_env_node(env_list, current, prev);
			break ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(char **args, t_env_list *env_list)
{
	int	i;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		unset_env_var(args[i], env_list);
		i++;
	}
	return (0);
}
