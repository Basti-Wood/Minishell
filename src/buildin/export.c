/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:02:12 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:00:04 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_export(char **args, t_env_list *env_list)
{
	int	i;

	if (!args[1])
	{
		print_export_env(env_list);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (!export_single_var(args[i], env_list))
			return (1);
		i++;
	}
	return (0);
}
