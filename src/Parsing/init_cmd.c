#include "../../include/minishell.h"

t_cmd *init_new_cmd(void)
{
	t_cmd *cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
	cmd->redirs = NULL;
	cmd->heredoc = 0;
	cmd->next = NULL;
	return (cmd);
}