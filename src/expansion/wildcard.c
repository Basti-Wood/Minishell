/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 16:00:00 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <glob.h>

int	count_glob_matches(char *pattern)
{
	glob_t	glob_result;
	int		count;

	if (glob(pattern, GLOB_NOCHECK | GLOB_TILDE, NULL, &glob_result) != 0)
		return (0);
	count = glob_result.gl_pathc;
	globfree(&glob_result);
	return (count);
}

static char	**allocate_result_array(int count)
{
	char	**result;

	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	result[count] = NULL;
	return (result);
}

static char	**copy_glob_results(glob_t *glob_result)
{
	char	**result;
	size_t	i;

	result = allocate_result_array(glob_result->gl_pathc);
	if (!result)
		return (NULL);
	i = 0;
	while (i < glob_result->gl_pathc)
	{
		result[i] = ft_strdup(glob_result->gl_pathv[i]);
		if (!result[i])
		{
			while (i > 0)
				free(result[--i]);
			free(result);
			return (NULL);
		}
		i++;
	}
	return (result);
}

char	**expand_wildcard(char *pattern)
{
	glob_t	glob_result;
	char	**result;

	if (!pattern || !ft_strchr(pattern, '*'))
	{
		result = allocate_result_array(1);
		if (!result)
			return (NULL);
		result[0] = ft_strdup(pattern);
		return (result);
	}
	if (glob(pattern, GLOB_NOCHECK | GLOB_TILDE, NULL, &glob_result) != 0)
	{
		result = allocate_result_array(1);
		if (!result)
			return (NULL);
		result[0] = ft_strdup(pattern);
		return (result);
	}
	result = copy_glob_results(&glob_result);
	globfree(&glob_result);
	return (result);
}

int	has_wildcard(char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '*')
			return (1);
		str++;
	}
	return (0);
}
