/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:03:44 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/05 12:35:52 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

static	t_env_node	*init_env(char **env)
{
	char		*equal_sign;
	t_env_node	*head;
	t_env_node	**current;
	t_env_node	*new_node;

	*head = NULL;
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
			current = &new_node->next;
		}
		env**;
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

	*envi = init_env(env);
	ret.head = envi;
	ret.size = env_size(envi);
	return (ret);
}
