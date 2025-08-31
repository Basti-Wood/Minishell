/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:15 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*create_env_string(t_env_node *node)
{
	char	*env_str;
	char	*tmp;

	if (node->value)
		env_str = ft_strjoin(node->key, "=");
	else
		env_str = ft_strdup(node->key);
	if (env_str && node->value)
	{
		tmp = env_str;
		env_str = ft_strjoin(env_str, node->value);
		free(tmp);
	}
	return (env_str);
}

static int	populate_env_array(t_shell *shell, char **env_array)
{
	t_env_node	*current;
	char		*env_str;
	int			env_count;

	env_count = 0;
	current = shell->env_list.head;
	while (current)
	{
		env_str = create_env_string(current);
		if (env_str)
			env_array[env_count++] = env_str;
		current = current->next;
	}
	env_array[env_count] = NULL;
	return (0);
}

int	create_env_array(t_shell *shell, char ***env_array)
{
	int	env_count;

	env_count = shell->env_list.size;
	*env_array = malloc(sizeof(char *) * (env_count + 1));
	if (!*env_array)
		return (1);
	return (populate_env_array(shell, *env_array));
}
