#include "../include/minishell.h"

int	validate_all_redirects(t_cmd *cmd)
{
	t_redir	*current;

	if (!cmd || !cmd->redirs)
		return (0);
	current = cmd->redirs;
	while (current)
	{
		if (current->type == REDIR_INPUT && current->filename)
		{
			if (access(current->filename, F_OK) == -1)
			{
				ft_fprintf_stderr("minishell: %s: No such file or directory\n",
					current->filename);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}