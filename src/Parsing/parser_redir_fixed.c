#include "../../include/minishell.h"

t_redir *create_redir(char *filename, t_redir_type type)
{
	t_redir *redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->filename = filename;
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

int handle_input_redir(t_token **tokens, t_cmd *cmd)
{
	t_redir *redir;
	char *filename;

	*tokens = (*tokens)->next;
	if (!*tokens || !(*tokens)->str)
	{
		ft_fprintf_stderr("minishell: syntax error near redirect\n");
		return (0);
	}
	filename = remove_quote_markers((*tokens)->str);
	if (!filename)
		return (0);
	redir = create_redir(filename, REDIR_INPUT);
	if (!redir)
	{
		free(filename);
		return (0);
	}
	add_to_ordered_redirs(cmd, redir);
	return (1);
}