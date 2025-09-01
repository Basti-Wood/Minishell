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

void	free_paths_array(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

static char	*handle_absolute_path(char *cmd)
{
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

char	*find_executable(char *cmd, t_env_list *env_list)
{
	char	*path_env;

	if (!cmd || !env_list)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (handle_absolute_path(cmd));
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	return (search_in_paths(cmd, path_env));
}

char	*build_full_path(char *dir, char *cmd)
{
	char	*full_path;
	size_t	path_len;
	size_t	cmd_len;

	path_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	full_path = malloc(path_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}
