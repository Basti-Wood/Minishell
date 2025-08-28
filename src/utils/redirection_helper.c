/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:00:00 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 16:42:52 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_to_ordered_redirs(t_cmd *cmd, t_redir *redir)
{
	t_redir	*current;

	if (!cmd->redirs)
	{
		cmd->redirs = redir;
		return ;
	}
	current = cmd->redirs;
	while (current->next)
		current = current->next;
	current->next = redir;
}

void	add_redirection(t_cmd *cmd, t_redir *redir)
{
	if (!redir)
		return ;
	if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
	{
		if (!cmd->infiles)
			cmd->infiles = redir;
		else
		{
			t_redir *current = cmd->infiles;
			while (current->next)
				current = current->next;
			current->next = redir;
		}
	}
	else
	{
		if (!cmd->outfiles)
			cmd->outfiles = redir;
		else
		{
			t_redir *current = cmd->outfiles;
			while (current->next)
				current = current->next;
			current->next = redir;
		}
	}
	add_to_ordered_redirs(cmd, redir);
}

int	validate_input_redir(t_redir *redir)
{
	if (access(redir->filename, F_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: No such file or directory\n",
			redir->filename);
		return (-1);
	}
	if (access(redir->filename, R_OK) == -1)
	{
		ft_fprintf_stderr("minishell: %s: Permission denied\n",
			redir->filename);
		return (-1);
	}
	return (0);
}

int	validate_output_redir(t_redir *redir)
{
	char	*dir;
	char	*last_slash;
	int		result;

	dir = ft_strdup(redir->filename);
	if (!dir)
		return (-1);
	last_slash = ft_strrchr(dir, '/');
	result = 0;
	if (last_slash)
	{
		*last_slash = '\0';
		if (access(dir, W_OK) == -1)
		{
			ft_fprintf_stderr("minishell: %s: Permission denied\n", dir);
			result = -1;
		}
	}
	else if (access(".", W_OK) == -1)
	{
		ft_fprintf_stderr("minishell: Permission denied\n");
		result = -1;
	}
	free(dir);
	return (result);
}

int	validate_single_redir(t_redir *redir)
{
	if (!redir)
		return (0);
	if (redir->type == REDIR_INPUT)
		return (validate_input_redir(redir));
	else if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
		return (validate_output_redir(redir));
	return (0);
}
