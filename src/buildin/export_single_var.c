/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_single_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 11:30:27 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/26 11:30:45 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	export_single_var(char *arg, t_env_list *env_list)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	else
	{
		*equal_sign = '\0';
		key = ft_strdup(arg);
		value = ft_strdup(equal_sign + 1);
		*equal_sign = '=';
	}
	if (!is_valid_identifier(key))
	{
		ft_fprintf_stderr("export: `%s': not a valid identifier\n", arg);
		free(key);
		free(value);
		return (0);
	}
	set_env_value(env_list, key, value);
	free(key);
	free(value);
	return (1);
}
