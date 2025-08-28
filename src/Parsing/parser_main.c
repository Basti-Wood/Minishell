#include "../../include/minishell.h"

int	process_token(t_token **tokens, t_cmd *cmd, int *argc, t_shell *shell)
{
	char	*cleaned_str;

	if (!tokens || !*tokens || !cmd || !argc)
		return (0);
	if ((*tokens)->type == CMD || (*tokens)->type == ARG)
	{
		cleaned_str = remove_quote_markers((*tokens)->str);
		if (!cleaned_str)
			return (0);
		if (!add_argument(cmd, cleaned_str, argc))
		{
			free(cleaned_str);
			return (0);
		}
		free(cleaned_str);
	}
	else if (is_redirection_token((*tokens)->type))
	{
		if (!handle_redirection(tokens, cmd, shell))
			return (0);
	}
	return (1);
}

t_cmd	*process_command_tokens(t_token **tokens, t_shell *shell)
{
	t_cmd	*new_cmd;
	int		argc;

	argc = 0;
	if (!tokens || !*tokens)
		return (NULL);
	new_cmd = init_new_cmd();
	if (!new_cmd)
		return (NULL);
	while (*tokens && (*tokens)->type != PIPE && (*tokens)->type != END)
	{
		if (!process_token(tokens, new_cmd, &argc, shell))
		{
			free_cmds(new_cmd);
			return (NULL);
		}
		*tokens = (*tokens)->next;
	}
	if (argc == 0 && !new_cmd->redirs && !new_cmd->infiles && !new_cmd->outfiles)
	{
		free_cmds(new_cmd);
		return (NULL);
	}
	return (new_cmd);
}

static void	add_cmd_to_pipeline(t_cmd **cmd_list,
								t_cmd **current_cmd, t_cmd *new_cmd)
{
	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		*current_cmd = new_cmd;
	}
	else
	{
		(*current_cmd)->next = new_cmd;
		*current_cmd = new_cmd;
	}
}

t_cmd	*parse_pipeline(t_token **tokens, t_shell *shell)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	t_cmd	*new_cmd;

	cmd_list = NULL;
	current_cmd = NULL;
	while (*tokens && (*tokens)->type != END)
	{
		new_cmd = process_command_tokens(tokens, shell);
		if (!new_cmd)
		{
			free_cmds(cmd_list);
			return (NULL);
		}
		add_cmd_to_pipeline(&cmd_list, &current_cmd, new_cmd);
		if (*tokens && (*tokens)->type == PIPE)
			*tokens = (*tokens)->next;
	}
	return (cmd_list);
}

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
