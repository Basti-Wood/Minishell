/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	remove_env_node(t_env_list *env_list, const char *key)
{
	t_env_node	*current;
	t_env_node	*prev;

	if (!env_list || !key)
		return ;
	current = env_list->head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				env_list->head = current->next;
			free(current->key);
			free(current->value);
			free(current);
			if (env_list->size > 0)
				env_list->size--;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(char **args, t_env_list *env_list)
{
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		remove_env_node(env_list, args[i]);
		i++;
	}
	return (0);
}
