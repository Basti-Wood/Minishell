#include "../../include/minishell.h"

int	handle_input_redir(t_token **tokens, t_cmd *cmd)
{
	char	*filename;
	t_redir	*redir;

	*tokens = (*tokens)->next;
	if (!*tokens || ((*tokens)->type != CMD && (*tokens)->type != ARG))
	{
		ft_fprintf_stderr("minishell: syntax error near unexpected token\n");
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
	add_to_infiles(cmd, redir);
	add_to_ordered_redirs(cmd, redir);
	return (1);
}

int	handle_output_redir(t_token **tokens, t_cmd *cmd)
{
	char			*filename;
	t_redir			*redir;
	t_redir_type	type;

	type = ((*tokens)->type == APPEND) ? REDIR_APPEND : REDIR_OUTPUT;
	*tokens = (*tokens)->next;
	if (!*tokens || ((*tokens)->type != CMD && (*tokens)->type != ARG))
	{
		ft_fprintf_stderr("minishell: syntax error near unexpected token\n");
		return (0);
	}
	filename = remove_quote_markers((*tokens)->str);
	if (!filename)
		return (0);
	redir = create_redir(filename, type);
	if (!redir)
	{
		free(filename);
		return (0);
	}
	add_to_outfiles(cmd, redir);
	add_to_ordered_redirs(cmd, redir);
	return (1);
}

int	handle_heredoc_redir(t_token **tokens, t_cmd *cmd, t_shell *shell)
{
	char	*delimiter;
	int		fd;

	*tokens = (*tokens)->next;
	if (!*tokens || ((*tokens)->type != CMD && (*tokens)->type != ARG))
	{
		ft_fprintf_stderr("minishell: syntax error near unexpected token\n");
		return (0);
	}
	delimiter = remove_quote_markers((*tokens)->str);
	if (!delimiter)
		return (0);
	fd = handle_heredoc(delimiter, shell);
	free(delimiter);
	if (fd < 0)
		return (0);
	cmd->heredoc = fd;
	return (1);
}

int	handle_redirection(t_token **tokens, t_cmd *cmd, t_shell *shell)
{
	if ((*tokens)->type == INPUT)
		return (handle_input_redir(tokens, cmd));
	else if ((*tokens)->type == TRUNC || (*tokens)->type == APPEND)
		return (handle_output_redir(tokens, cmd));
	else if ((*tokens)->type == HEREDOC)
		return (handle_heredoc_redir(tokens, cmd, shell));
	return (0);
}

int	is_redirection_token(t_token_type type)
{
	return (type == INPUT || type == TRUNC || 
			type == APPEND || type == HEREDOC);
}
