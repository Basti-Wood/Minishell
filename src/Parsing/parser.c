#include "../../include/minishell.h"

// For multiple output redirections, create all files but only the last is used for output
static void handle_multiple_redirections(t_cmd *cmd, t_token *token)
{
    if (cmd->outfile)
    {
        int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
        int fd = open(cmd->outfile, flags, 0644);
        if (fd != -1)
            close(fd);
        free(cmd->outfile);
    }
    cmd->outfile = remove_quote_markers(token->str);
    cmd->append = (token->prev->type == APPEND);
}
#include "../../include/minishell.h"


t_cmd *parse_tokens(t_token *tokens, t_shell *shell)
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
            new_cmd->heredoc = -1;
            int argc = 0;

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
                    handle_multiple_redirections(new_cmd, tokens);
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
                        free(new_cmd->argv);
                        free(new_cmd);
                        return NULL;
                    }
                }
                else if (tokens->type == INPUT || tokens->type == TRUNC || 
                         tokens->type == APPEND || tokens->type == HEREDOC)
                {
                    fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", 
                            tokens->next ? tokens->next->str : "newline");
                    shell->exit_status = 258;
                        free(new_cmd->argv);
                        free(new_cmd);
                    return NULL;
                }
                else
                {
                    break;
                }
                tokens = tokens->next;
            }

            if (argc == 0)
            {
                fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", 
                        tokens && tokens->str ? tokens->str : "newline");
                shell->exit_status = 258;
                    free(new_cmd->argv);
                    free(new_cmd);
                return NULL;
            }

            if (!head)
                head = new_cmd;
            else
                current->next = new_cmd;
            current = new_cmd;
        }
        else if (tokens->type == INPUT || tokens->type == TRUNC || 
                 tokens->type == APPEND || tokens->type == HEREDOC)
        {
            fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokens->str);
            shell->exit_status = 258;
            return NULL;
        }
        else
        {
            tokens = tokens->next;
            continue;
        }

        if (tokens && tokens->type == PIPE)
            tokens = tokens->next;
    }

    return head;
}