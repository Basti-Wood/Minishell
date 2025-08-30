#include "../../include/minishell.h"

static t_redir_type	get_output_type(t_token_type token_type)
{
	if (token_type == APPEND)
		return (REDIR_APPEND);
	return (REDIR_OUTPUT);
}

int	handle_output_redir(t_token **tokens, t_cmd *cmd)
{
	t_redir_type	type;
	t_redir			*redir;
	char			*filename;
	t_token			*next_token;

	if (!tokens || !*tokens || !cmd)
		return (-1);
	type = get_output_type((*tokens)->type);
	next_token = (*tokens)->next;
	if (!next_token || !next_token->str)
	{
		ft_fprintf_stderr("minishell: syntax error near redirect\n");
		return (-1);
	}
	filename = remove_quote_markers(next_token->str);
	if (!filename)
		return (-1);
	redir = create_redir(filename, type);
	if (!redir)
	{
		free(filename);
		return (-1);
	}
	add_to_ordered_redirs(cmd, redir);
	return (2);
}

static int	close_previous_heredoc(t_cmd *cmd)
{
	if (cmd->heredoc > 0)
	{
		close(cmd->heredoc);
		cmd->heredoc = -1;
	}
	return (0);
}

int	handle_heredoc_redir(t_token **tokens, t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;
	char	*delimiter;
	t_token	*next_token;

	if (!tokens || !*tokens || !cmd || !shell)
		return (-1);
	next_token = (*tokens)->next;
	if (!next_token || !next_token->str)
		return (-1);
	close_previous_heredoc(cmd);
	delimiter = remove_quote_markers(next_token->str);
	if (!delimiter)
		return (-1);
	cmd->heredoc = handle_heredoc(delimiter, shell);
	free(delimiter);
	if (cmd->heredoc == -1)
		return (-1);
	redir = create_redir(NULL, REDIR_HEREDOC);
	if (!redir)
	{
		close(cmd->heredoc);
		cmd->heredoc = -1;
		return (-1);
	}
	add_to_ordered_redirs(cmd, redir);
	return (2);
}