/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_validation_fixed.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_file_access(char *filename, int mode)
{
	if (access(filename, F_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: No such file or directory\n",
			filename);
		return (1);
	}
	if (access(filename, mode) == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n", filename);
		return (1);
	}
	return (0);
}

int	validate_input_file_redir(t_redir *redir)
{
	if (!redir || !redir->filename)
		return (1);
	return (check_file_access(redir->filename, R_OK));
}

int	validate_output_file_redir(t_redir *redir)
{
	char	*dir;
	char	*last_slash;
	int		result;

	if (!redir || !redir->filename)
		return (1);
	dir = ft_strdup(redir->filename);
	if (!dir)
		return (1);
	last_slash = ft_strrchr(dir, '/');
	if (last_slash)
	{
		*last_slash = '\0';
		result = check_file_access(dir, W_OK);
	}
	else
		result = check_file_access(".", W_OK);
	free(dir);
	return (result);
}
