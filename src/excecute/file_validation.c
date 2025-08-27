/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_validation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:54:15 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	validate_input_files_before_output(t_cmd *cmd)
{
	if (cmd->redirs && validate_redir_list(cmd->redirs) == -1)
		return (-1);
	if (cmd->infiles && validate_redir_list(cmd->infiles) == -1)
		return (-1);
	return (0);
}

int	validate_output_file(const char *filename)
{
	char	*dir;
	char	*last_slash;
	int		result;

	dir = ft_strdup(filename);
	last_slash = ft_strrchr(dir, '/');
	result = 1;
	if (last_slash)
	{
		*last_slash = '\0';
		if (access(dir, W_OK) == -1)
		{
			ft_fprintf_stderr("minishell: %s: Permission denied\n", dir);
			result = 0;
		}
	}
	free(dir);
	return (result);
}
