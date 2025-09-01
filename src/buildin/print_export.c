/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 11:31:10 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/26 11:31:21 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	swap_env_nodes(t_env_node *a, t_env_node *b)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = a->key;
	temp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = temp_key;
	b->value = temp_value;
}

void	sort_env_list(t_env_list *env_list)
{
	t_env_node	*i;
	t_env_node	*j;

	if (!env_list || !env_list->head)
		return ;
	i = env_list->head;
	while (i)
	{
		j = i->next;
		while (j)
		{
			if (strcmp(i->key, j->key) > 0)
				swap_env_nodes(i, j);
			j = j->next;
		}
		i = i->next;
	}
}

void	print_export_env(t_env_list *env_list)
{
	t_env_node	*current;

	if (!env_list || !env_list->head)
		return ;
	sort_env_list(env_list);
	current = env_list->head;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
}
