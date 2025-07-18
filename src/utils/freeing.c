#include "../include/minishell.h"

#if 0
void free_redirs(t_redir *redirs) {
    t_redir *tmp;
    while (redirs) {
        tmp = redirs->next;
        if (redirs->filename)
            free(redirs->filename);
        free(redirs);
        redirs = tmp;
    }
}
#endif
#include "../include/minishell.h"


void free_tokens(t_token *tokens)
{
    t_token *tmp;
    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->str);
        free(tmp);
    }
}

void free_cmds(t_cmd *cmds)
{
    t_cmd *tmp;
    while (cmds)
    {
        tmp = cmds;
        cmds = cmds->next;
        if (tmp->argv)
        {
            int i = 0;
            while (tmp->argv[i])
                free(tmp->argv[i++]);
            free(tmp->argv);
        }
        if (tmp->infile)
            free(tmp->infile);
        if (tmp->outfile)
            free(tmp->outfile);
        if (tmp->heredoc > 0)
            close(tmp->heredoc);
        free(tmp);
    }
}

void free_env_list(t_env_list *env_list)
{
    t_env_node *current = env_list->head;
    t_env_node *tmp;
    while (current)
    {
        tmp = current;
        current = current->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
    free(env_list);
}



