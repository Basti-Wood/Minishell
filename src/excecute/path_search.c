/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_search.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 19:20:11 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 19:21:38 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*try_path_access(char *dir, char *cmd, char **paths)
{
	char	*full_path;

	full_path = build_full_path(dir, cmd);
	if (!full_path)
	{
		free_paths_array(paths);
		return (NULL);
	}
	if (access(full_path, X_OK) == 0)
	{
		free_paths_array(paths);
		return (full_path);
	}
	free(full_path);
	return (NULL);
}

char	*search_in_paths(char *cmd, char *path_env)
{
	char	**paths;
	char	*result;
	int		i;

	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		result = try_path_access(paths[i], cmd, paths);
		if (result)
			return (result);
		i++;
	}
	free_paths_array(paths);
	return (NULL);
}
