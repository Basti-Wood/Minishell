#include "../include/minishell.h"

static int is_valid_identifier(const char *str)
{
    if (!str || !*str)
        return 0;
    
    // First character must be letter or underscore
    if (!ft_isalpha(str[0]) && str[0] != '_')
        return 0;
    
    // Rest can be alphanumeric or underscore
    for (int i = 1; str[i]; i++)
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return 0;
    }
    
    return 1;
}

int builtin_export(char **args, t_env_list *env_list)
{
    int i = 1;
    int exit_status = 0;
    char *key, *value;
    char *equal_sign;

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

    while (args[i])
    {
        equal_sign = ft_strchr(args[i], '=');
        
        if (equal_sign)
        {
            *equal_sign = '\0';
            key = args[i];
            value = equal_sign + 1;
            
            if (!is_valid_identifier(key))
            {
                ft_fprintf_stderr("minishell: export: `%s': not a valid identifier\n", key);
                exit_status = 1;
            }
            else
            {
                set_env_value(env_list, key, value);
            }
            
            *equal_sign = '=';
        }
        else
        {
            key = args[i];
            
            if (!is_valid_identifier(key))
            {
                ft_fprintf_stderr("minishell: export: `%s': not a valid identifier\n", key);
                exit_status = 1;
            }
            else if (!get_env_value(env_list, key))
            {
                set_env_value(env_list, key, "");
            }
        }
        i++;
    }
    
    return exit_status;
}