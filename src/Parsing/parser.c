#include "../../include/minishell.h"

t_cmd *parse_tokens(t_token *tokens)
{
	t_cmd *head = NULL;
	t_cmd *current = NULL;

	while (tokens)
	{
		if (tokens->type == CMD)
		{
			t_cmd *new_cmd = malloc(sizeof(t_cmd));
			ft_bzero(new_cmd, sizeof(t_cmd));
			new_cmd->argv = ft_calloc(1, sizeof(char *));
			int argc = 0;

			while (tokens && tokens->type != PIPE && tokens->type != END)
			{
				if (tokens->type == CMD || tokens->type == ARG)
				{
					new_cmd->argv = realloc(new_cmd->argv, (argc + 2) * sizeof(char *));
					new_cmd->argv[argc++] = ft_strdup(tokens->str);
					new_cmd->argv[argc] = NULL;
				}
				else if (tokens->type == INPUT && tokens->next)
				{
					tokens = tokens->next;
					new_cmd->infile = ft_strdup(tokens->str);
				}
				else if ((tokens->type == TRUNC || tokens->type == APPEND) && tokens->next)
				{
					tokens = tokens->next;
					new_cmd->outfile = ft_strdup(tokens->str);
					new_cmd->append = (tokens->prev->type == APPEND);
				}
				tokens = tokens->next;
			}

			if (!head)
				head = new_cmd;
			else
				current->next = new_cmd;
			current = new_cmd;
		}

		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
	}

	return head;
}
