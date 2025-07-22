#include "../../include/minishell.h"

t_cmd *parse_tokens(t_token *tokens, t_shell *shell)
{
    t_cmd *head = NULL;
    t_cmd *current = NULL;

    while (tokens)
    {
        if (tokens->type == PIPE)
        {
            if (!current)
            {
                ft_fprintf_stderr("minishell: syntax error near unexpected token `|'\n");
                shell->exit_status = 258;
                free_cmds(head);
                return NULL;
            }
            tokens = tokens->next;
            continue;
        }

        if (tokens->type == CMD || tokens->type == ARG ||
            tokens->type == INPUT || tokens->type == TRUNC || 
            tokens->type == APPEND || tokens->type == HEREDOC)
        {
            t_cmd *new_cmd = malloc(sizeof(t_cmd));
            ft_bzero(new_cmd, sizeof(t_cmd));
            new_cmd->argv = ft_calloc(1, sizeof(char *));
            new_cmd->heredoc = -1;
            int argc = 0;

            // Initialize redirection lists for this command
            new_cmd->infiles = NULL;
            new_cmd->outfiles = NULL;
            new_cmd->redirs = NULL;  // This will maintain the order

            while (tokens && tokens->type != PIPE && tokens->type != END)
            {
                if (tokens->type == CMD || tokens->type == ARG)
                {
                    new_cmd->argv = realloc(new_cmd->argv, (argc + 2) * sizeof(char *));
                    new_cmd->argv[argc++] = remove_quote_markers(tokens->str);
                    new_cmd->argv[argc] = NULL;
                }
                else if (tokens->type == INPUT && tokens->next)
                {
                    tokens = tokens->next;
                    t_redir *redir = malloc(sizeof(t_redir));
                    if (!redir) {
                        perror("malloc");
                        free_cmds(new_cmd);
                        free_cmds(head);
                        return NULL;
                    }
                    redir->filename = remove_quote_markers(tokens->str);
                    redir->type = REDIR_INPUT;
                    redir->next = NULL;
                    
                    // Add to ordered redirs list (PRIMARY LIST)
                    t_redir **last_ordered = &new_cmd->redirs;
                    while (*last_ordered) last_ordered = &(*last_ordered)->next;
                    *last_ordered = redir;
                    
                    // Also add to infiles list for backward compatibility
                    t_redir *infile_redir = malloc(sizeof(t_redir));
                    if (!infile_redir) {
                        perror("malloc");
                        free_cmds(new_cmd);
                        free_cmds(head);
                        return NULL;
                    }
                    infile_redir->filename = ft_strdup(redir->filename);
                    infile_redir->type = REDIR_INPUT;
                    infile_redir->next = NULL;
                    t_redir **last = &new_cmd->infiles;
                    while (*last) last = &(*last)->next;
                    *last = infile_redir;
                }
                else if ((tokens->type == TRUNC || tokens->type == APPEND) && tokens->next)
                {
                    tokens = tokens->next;
                    t_redir *redir = malloc(sizeof(t_redir));
                    if (!redir) {
                        perror("malloc");
                        free_cmds(new_cmd);
                        free_cmds(head);
                        return NULL;
                    }
                    redir->filename = remove_quote_markers(tokens->str);
                    redir->type = (tokens->prev->type == APPEND) ? REDIR_APPEND : REDIR_OUTPUT;
                    redir->next = NULL;
                    
                    // Add to ordered redirs list (PRIMARY LIST)
                    t_redir **last_ordered = &new_cmd->redirs;
                    while (*last_ordered) last_ordered = &(*last_ordered)->next;
                    *last_ordered = redir;
                    
                    // Also add to outfiles list for backward compatibility
                    t_redir *outfile_redir = malloc(sizeof(t_redir));
                    if (!outfile_redir) {
                        perror("malloc");
                        free_cmds(new_cmd);
                        free_cmds(head);
                        return NULL;
                    }
                    outfile_redir->filename = ft_strdup(redir->filename);
                    outfile_redir->type = redir->type;
                    outfile_redir->next = NULL;
                    t_redir **last = &new_cmd->outfiles;
                    while (*last) last = &(*last)->next;
                    *last = outfile_redir;
                }
                else if (tokens->type == HEREDOC && tokens->next)
                {
                    tokens = tokens->next;
                    if (new_cmd->heredoc > 0)
                        close(new_cmd->heredoc);
                    char *delimiter = remove_quote_markers(tokens->str);
                    new_cmd->heredoc = handle_heredoc(delimiter, shell);
                    free(delimiter);
                    if (new_cmd->heredoc == -1)
                    {
                        shell->exit_status = 130;
                        free_cmds(new_cmd);
                        free_cmds(head);
                        return NULL;
                    }
                    
                    // Add heredoc to ordered redirs list (PRIMARY LIST)
                    t_redir *redir = malloc(sizeof(t_redir));
                    if (!redir) {
                        perror("malloc");
                        free_cmds(new_cmd);
                        free_cmds(head);
                        return NULL;
                    }
                    redir->filename = NULL;  // Heredoc doesn't use filename
                    redir->type = REDIR_HEREDOC;
                    redir->next = NULL;
                    t_redir **last_ordered = &new_cmd->redirs;
                    while (*last_ordered) last_ordered = &(*last_ordered)->next;
                    *last_ordered = redir;
                }
                else if (tokens->type == INPUT || tokens->type == TRUNC || 
                         tokens->type == APPEND || tokens->type == HEREDOC)
                {
                    ft_fprintf_stderr("minishell: syntax error near unexpected token `%s'\n", 
                            tokens->next ? tokens->next->str : "newline");
                    shell->exit_status = 258;
                    free_cmds(new_cmd);
                    free_cmds(head);
                    return NULL;
                }
                tokens = tokens->next;
            }

            // Allow empty commands with redirections - check the ordered list
            if (argc == 0 && !new_cmd->redirs)
            {
                free(new_cmd->argv);
                free_cmds(new_cmd);
                continue;
            }

            if (!head)
                head = new_cmd;
            else
                current->next = new_cmd;
            current = new_cmd;
        }
        else
        {
            tokens = tokens->next;
        }

        if (tokens && tokens->type == PIPE)
            tokens = tokens->next;
    }

    return head;
}