/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*handle_exit_status(char **src, char *dst, t_shell *shell)
{
	char	*temp;

	temp = ft_itoa(shell->exit_status);
	dst = copy_string_to_dst(dst, temp);
	free(temp);
	(*src)++;
	return (dst);
}

static char	*handle_variable(char **src, char *dst, t_shell *shell)
{
	char	*var_name;
	char	*var_value;

	var_name = get_var_name(src);
	var_value = get_env_value(&shell->env_list, var_name);
	if (var_value)
		dst = copy_string_to_dst(dst, var_value);
	free(var_name);
	return (dst);
}

char	*handle_special_chars(char **src, char *dst)
{
	if (**src == '?')
		(*src)++;
	else if (ft_isdigit(**src))
		(*src)++;
	else
		*dst++ = '$';
	return (dst);
}

char	*expand_dollar(char **src, char *dst, t_shell *shell)
{
	(*src)++;
	if (!**src || ft_isspace(**src))
		*dst++ = '$';
	else if (**src == '?')
		dst = handle_exit_status(src, dst, shell);
	else if (ft_isalpha(**src) || **src == '_')
		dst = handle_variable(src, dst, shell);
	else if (ft_isdigit(**src))
	{
		(*src)++;
		return (dst);
	}
	else if (**src == '$' || **src == '\'' || **src == '\"')
		return (dst);
	else
		*dst++ = '$';
	return (dst);
}
