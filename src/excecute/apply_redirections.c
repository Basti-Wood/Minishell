#include "../../include/minishell.h"

int apply_redirections(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	return (handle_redirections_in_order(cmd));
}
