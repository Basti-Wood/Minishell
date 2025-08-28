#include "../../include/minishell.h"

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		if (tmp->argv)
			free_argv(tmp->argv);
		free_redirs(tmp->infiles);
		free_redirs(tmp->outfiles);
		free_redirs(tmp->redirs);
		if (tmp->heredoc > 0)
			close(tmp->heredoc);
		free(tmp);
	}
}
