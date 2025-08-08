/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/01 15:55:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	remove_env_node(
				t_env_list *env_list, t_env_node *current, t_env_node *prev)
{
	if (prev)
		prev->next = current->next;
	else
		env_list->head = current->next;
	free(current->key);
	free(current->value);
	free(current);
	env_list->size--;
}

int	builtin_unset(char **args, t_env_list *env_list)
{
	int			i;
	t_env_node	*current;
	t_env_node	*prev;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		current = env_list->head;
		prev = NULL;
		while (current)
		{
			if (ft_strcmp(current->key, args[i]) == 0)
			{
				remove_env_node(env_list, current, prev);
				break ;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
	return (0);
}
