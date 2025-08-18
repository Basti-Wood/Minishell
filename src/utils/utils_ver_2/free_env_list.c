/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/18 17:01:46 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_env_list(t_env_list *env_list)
{
	t_env_node	*current;
	t_env_node	*tmp;

	current = env_list->head;
	while (current)
	{
		tmp = current;
		current = current->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
	free(env_list);
}
