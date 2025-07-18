#include "../../include/minishell.h"

static int validate_and_create_redirections(t_redir_check *redirs)
{
    t_redir_check *current = redirs;
    while (current)
    {
        int flags = O_WRONLY | O_CREAT | (current->append ? O_APPEND : O_TRUNC);
        int fd = open(current->filename, flags, 0644);
        if (fd == -1)
        {
            // Log the issue but do not abort parsing
            perror(current->filename);
        }
        else
        {
            close(fd);
        }
        current = current->next;
    }
    return 0;
}

static void free_redir_check(t_redir_check *redirs)
{
    t_redir_check *tmp;
    while (redirs)
    {
        tmp = redirs;
        redirs = redirs->next;
        free(tmp->filename);
        free(tmp);
    }
}

t_cmd *parse_tokens(t_token *tokens, t_shell *shell)
{
    t_cmd *head = NULL;
    t_cmd *current = NULL;
    t_redir_check *redirs = NULL;

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

            // Initialize redirection list for this command
            redirs = NULL;

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
                    if (new_cmd->infile)
                        free(new_cmd->infile);
                    new_cmd->infile = remove_quote_markers(tokens->str);
                }
                else if ((tokens->type == TRUNC || tokens->type == APPEND) && tokens->next)
                {
                    tokens = tokens->next;
                    // Store redirection for validation
                    t_redir_check *new_redir = malloc(sizeof(t_redir_check));
                    if (!new_redir)
                    {
                        perror("malloc");
                        free_redir_check(redirs);
                        free_cmds(new_cmd);
                        free_cmds(head);
                        return NULL;
                    }
                    new_redir->filename = remove_quote_markers(tokens->str);
                    new_redir->append = (tokens->prev->type == APPEND);
                    new_redir->next = redirs;
                    redirs = new_redir;

                    // Store last redirection in command
                    if (new_cmd->outfile)
                        free(new_cmd->outfile);
                    new_cmd->outfile = ft_strdup(new_redir->filename);
                    new_cmd->append = new_redir->append;
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
                        free_redir_check(redirs);
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
                    free_redir_check(redirs);
                    free_cmds(new_cmd);
                    free_cmds(head);
                    return NULL;
                }
                tokens = tokens->next;
            }

            // Validate output redirections
            if (redirs)
            {
				validate_and_create_redirections(redirs);
				free_redir_check(redirs);
            }

            // Allow empty commands with redirections
            if (argc == 0 && !new_cmd->outfile && !new_cmd->infile && new_cmd->heredoc <= 0)
            {
                free(new_cmd->argv);
                free(new_cmd);
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