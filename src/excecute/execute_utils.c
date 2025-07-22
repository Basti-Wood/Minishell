#include "../include/minishell.h"

static int	build_env_entry(t_env_node *node, char **env_array, int i)
{
	size_t	key_len;
	size_t	value_len;

	if (!node->key || !node->value)
		return (0);
	key_len = ft_strlen(node->key);
	value_len = ft_strlen(node->value);
	env_array[i] = malloc(key_len + value_len + 2);
	if (!env_array[i])
		return (-1);
	ft_strcpy(env_array[i], node->key);
	ft_strcat(env_array[i], "=");
	ft_strcat(env_array[i], node->value);
	return (1);
}

char	**env_list_to_array(t_env_list *env_list)
{
	char		**env_array;
	t_env_node	*current;
	int			i;
	int			j;

	if (!env_list || !env_list->head)
		return (NULL);
	env_array = malloc(sizeof(char *) * (env_list->size + 1));
	if (!env_array)
		return (NULL);
	current = env_list->head;
	i = 0;
	while (current && i < env_list->size)
	{
		if (build_env_entry(current, env_array, i) == -1)
		{
			j = 0;
			while (j < i)
				free(env_array[j++]);
			free(env_array);
			return (NULL);
		}
		if (env_array[i])
			i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

void	shift_argv(char ***argv)
{
	int	i;
	int	j;

	i = 0;
	while ((*argv)[i] && (*argv)[i][0] == '\0')
	{
		free((*argv)[i]);
		i++;
	}
	if (!(*argv)[i])
	{
		free((*argv)[0]);
		(*argv)[0] = NULL;
		return ;
	}
	j = 0;
	while ((*argv)[i])
	{
		(*argv)[j] = (*argv)[i];
		j++;
		i++;
	}
	(*argv)[j] = NULL;
}
