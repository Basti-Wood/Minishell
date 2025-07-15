#include "../include/minishell.h"

char *get_env_value(t_env_list *env_list, const char *key)
{
    t_env_node *current = env_list->head;
    
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return current->value;
        current = current->next;
    }
    
    return NULL;
}

void set_env_value(t_env_list *env_list, const char *key, const char *value)
{
    t_env_node *current = env_list->head;

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = ft_strdup(value);
            return;
        }
        current = current->next;
    }

    t_env_node *new_node = malloc(sizeof(t_env_node));
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = env_list->head;
    env_list->head = new_node;
    env_list->size++;
}

int builtin_env(t_env_list *env_list)
{
    t_env_node *current = env_list->head;
    
    while (current)
    {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
    
    return 0;
}