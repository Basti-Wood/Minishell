#include "../../include/minishell.h"

t_cmd	*init_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
	cmd->redirs = NULL;
	cmd->heredoc = -1;
	cmd->next = NULL;
	return (cmd);
}

t_redir	*create_redir(char *filename, t_redir_type type)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->filename = filename;
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

void	add_to_ordered_redirs(t_cmd *cmd, t_redir *redir)
{
	t_redir	*current;

	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		current = cmd->redirs;
		while (current->next)
			current = current->next;
		current->next = redir;
	}
}

void	add_to_infiles(t_cmd *cmd, t_redir *redir)
{
	t_redir	*current;

	if (!cmd->infiles)
		cmd->infiles = redir;
	else
	{
		current = cmd->infiles;
		while (current->next)
			current = current->next;
		current->next = redir;
	}
}

void	add_to_outfiles(t_cmd *cmd, t_redir *redir)
{
	t_redir	*current;

	if (!cmd->outfiles)
		cmd->outfiles = redir;
	else
	{
		current = cmd->outfiles;
		while (current->next)
			current = current->next;
		current->next = redir;
	}
}
