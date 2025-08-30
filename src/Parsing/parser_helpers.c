#include "../../include/minishell.h"

t_cmd	*parse_command_line(t_token *tokens, t_shell *shell)
{
	t_cmd	*cmd_list;
	t_token	*current;

	if (!tokens || !shell)
		return (NULL);
	current = tokens;
	cmd_list = parse_pipeline(&current, shell);
	return (cmd_list);
}