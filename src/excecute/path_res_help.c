/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:53:42 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 15:54:03 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_executable(char *full_path)
{
	struct stat	st;

	if (access(full_path, F_OK) != 0)
		return (0);
	if (stat(full_path, &st) != 0)
		return (0);
	return (!S_ISDIR(st.st_mode));
}

static char	*check_single_path(char *path_dir, char *cmd)
{
	char	*full_path;

	full_path = build_full_path(path_dir, cmd);
	if (!full_path)
		return (NULL);
	if (is_valid_executable(full_path))
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*search_in_paths(char *cmd, char *path_env)
{
	char	**paths;
	char	*result;
	int		i;

	if (!cmd || !path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		result = check_single_path(paths[i], cmd);
		if (result)
		{
			free_paths_array(paths);
			return (result);
		}
		i++;
	}
	free_paths_array(paths);
	return (NULL);
}
