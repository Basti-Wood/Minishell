#include "../include/minishell.h"

int builtin_unset(char **args, t_env_list *env_list)
{
    int i = 1;
    t_env_node *current, *prev;
    
    if (!args[1])
        return 0;
    
    while (args[i])
    {
        current = env_list->head;
        prev = NULL;
        
        while (current)
        {
            if (ft_strcmp(current->key, args[i]) == 0)
            {
                if (prev)
                    prev->next = current->next;
                else
                    env_list->head = current->next;
                
                free(current->key);
                free(current->value);
                free(current);
                env_list->size--;
                break;
            }
            prev = current;
            current = current->next;
        }
        i++;
    }
    
    return 0;
}