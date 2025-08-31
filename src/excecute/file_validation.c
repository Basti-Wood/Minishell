/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:53:42 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 15:54:03 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_redirect_errors(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_INPUT)
		{
			if (access(current->filename, F_OK) == -1)
			{
				ft_fprintf_stderr("minishell: %s: No such file or directory\n",
					current->filename);
				return (1);
			}
			if (access(current->filename, R_OK) == -1)
			{
				ft_fprintf_stderr("minishell: %s: Permission denied\n",
					current->filename);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

int	validate_input_files_before_output(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (check_redirect_errors(cmd->redirs) != 0)
		return (1);
	return (0);
}

static int	check_file_is_directory(const char *filename)
{
	struct stat	st;

	if (stat(filename, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			ft_fprintf_stderr("%s: Is a directory\n", filename);
			return (1);
		}
	}
	return (0);
}

static int	validate_parent_directory(const char *filename)
{
	char	*dir_path;
	char	*last_slash;
	int		result;

	dir_path = ft_strdup(filename);
	if (!dir_path)
		return (1);
	last_slash = ft_strrchr(dir_path, '/');
	if (!last_slash || last_slash == dir_path)
	{
		free(dir_path);
		return (0);
	}
	*last_slash = '\0';
	if (access(dir_path, F_OK) == -1)
	{
		ft_fprintf_stderr("%s: No such file or directory\n", filename);
		result = 1;
	}
	else
		result = 0;
	free(dir_path);
	return (result);
}

int	validate_output_file(const char *filename)
{
	if (check_file_is_directory(filename) != 0)
		return (1);
	if (validate_parent_directory(filename) != 0)
		return (1);
	return (0);
}
