/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:03:15 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:08:29 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static	t_env_node	*init_env(char **env)
{
	t_env_node	*head;
	t_env_node	**current;
	t_env_node	*new_node;
	char		*equal_sign;

	head = NULL;
	current = &head;
	while (*env)
	{
		equal_sign = ft_strchr(*env, '=');
		if (equal_sign)
		{
			new_node = malloc(sizeof(t_env_node));
			if (!new_node)
				return (head);
			new_node->key = ft_strndup(*env, equal_sign - *env);
			new_node->value = ft_strdup(equal_sign + 1);
			new_node->next = NULL;
			*current = new_node;
			current = &(new_node->next);
		}
		env++;
	}
	return (head);
}

t_env_list	init_env_list(char **env)
{
	t_env_list	env_list;
	t_env_node	*current;
	int			count;

	env_list.head = init_env(env);
	count = 0;
	current = env_list.head;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_list.size = count;
	return (env_list);
}
