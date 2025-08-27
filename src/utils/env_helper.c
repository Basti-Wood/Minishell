/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enve helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/18 17:00:22 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(t_env_list *env_list, const char *key)
{
	t_env_node	*current;

	if (!env_list || !key)
		return (NULL);
	current = env_list->head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	set_env_value(t_env_list *env_list, const char *key, const char *value)
{
	t_env_node	*current;
	t_env_node	*new_node;

	if (!env_list || !key)
		return ;
	current = env_list->head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return ;
		}
		current = current->next;
	}
	new_node = create_env_node(key, value);
	if (!new_node)
		return ;
	new_node->next = env_list->head;
	env_list->head = new_node;
	env_list->size++;
}

t_env_node	*create_env_node(const char *key, const char *value)
{
	t_env_node	*new_node;

	new_node = malloc(sizeof(t_env_node));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	if (value)
	{
		new_node->value = ft_strdup(value);
		if (!new_node->value)
		{
			free(new_node->key);
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}
