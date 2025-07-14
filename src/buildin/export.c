#include "../include/minishell.h"

int builtin_export(char **args, t_env_list *env_list)
{
    int i = 1;
    char *key, *value;
    char *equal_sign;
    
    // If no arguments, print all environment variables
    if (!args[1])
    {
        t_env_node *current = env_list->head;
        while (current)
        {
            printf("declare -x %s=\"%s\"\n", current->key, current->value);
            current = current->next;
        }
        return 0;
    }
    
    // Process each argument
    while (args[i])
    {
        equal_sign = ft_strchr(args[i], '=');
        if (equal_sign)
        {
            // Variable assignment
            *equal_sign = '\0';
            key = args[i];
            value = equal_sign + 1;
            set_env_value(env_list, key, value);
            *equal_sign = '='; // Restore the string
        }
        else
        {
            // Just export existing variable or create empty one
            key = args[i];
            if (!get_env_value(env_list, key))
            {
                set_env_value(env_list, key, "");
            }
        }
        i++;
    }
    
    return 0;
}