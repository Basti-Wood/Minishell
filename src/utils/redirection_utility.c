#include "../../include/minishell.h"

int validate_redir_list(t_redir *redirs)
{
	t_redir *current;

	if (!redirs)
		return (0);
	current = redirs;
	while (current)
	{
		if (validate_single_redir(current) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}

void free_string_array(char **array)
{
	int i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}