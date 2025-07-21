#include "../../include/minishell.h"


t_cmd *parse_tokens(t_token *tokens, t_shell *shell)
{
    t_cmd *head = NULL;
    t_cmd *current = NULL;
    // t_redir_check *redirs = NULL; // removed obsolete variable

    while (tokens)
    {
        if (tokens->type == PIPE)
        {
            if (!current)
            {
                fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
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
                    // Append to infiles list
                    t_redir **last = &new_cmd->infiles;
                    while (*last) last = &(*last)->next;
                    *last = redir;
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
                    // Append to outfiles list
                    t_redir **last = &new_cmd->outfiles;
                    while (*last) last = &(*last)->next;
                    *last = redir;
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
                }
                else if (tokens->type == INPUT || tokens->type == TRUNC || 
                         tokens->type == APPEND || tokens->type == HEREDOC)
                {
                    fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", 
                            tokens->next ? tokens->next->str : "newline");
                    shell->exit_status = 258;
                    free_cmds(new_cmd);
                    free_cmds(head);
                    return NULL;
                }
                tokens = tokens->next;
            }

            // Allow empty commands with redirections
            if (argc == 0 && !new_cmd->outfiles && !new_cmd->infiles && new_cmd->heredoc <= 0)
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