/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:59:04 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 14:59:37 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*create_env_string(char *key, char *value)
{
	char	*env_str;
	size_t	key_len;
	size_t	value_len;

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	env_str = malloc(key_len + value_len + 2);
	if (!env_str)
		return (NULL);
	ft_strcpy(env_str, key);
	ft_strcat(env_str, "=");
	ft_strcat(env_str, value);
	return (env_str);
}

static void	free_env_array_partial(char **env_array, int count)
{
	int	j;

	j = 0;
	while (j < count)
		free(env_array[j++]);
	free(env_array);
}

char	**env_list_to_array(t_env_list *env_list)
{
	char		**env_array;
	t_env_node	*current;
	int			i;

	if (!env_list || !env_list->head)
		return (NULL);
	env_array = malloc(sizeof(char *) * (env_list->size + 1));
	if (!env_array)
		return (NULL);
	current = env_list->head;
	i = 0;
	while (current && i < env_list->size)
	{
		if (current->key && current->value)
		{
			env_array[i] = create_env_string(current->key, current->value);
			if (!env_array[i])
				return (free_env_array_partial(env_array, i), NULL);
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
