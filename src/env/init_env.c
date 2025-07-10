#include "../include/minishell.h"

static t_env_node *init_env(char **env) {
    t_env_node *head = NULL;
    t_env_node *current = NULL;

    for (int i = 0; env[i] != NULL; i++) {
        char *equal_sign = strchr(env[i], '=');
        if (!equal_sign)
            continue;

        t_env_node *new_node = malloc(sizeof(t_env_node));
        if (!new_node)
            return NULL;

        size_t key_len = equal_sign - env[i];
        new_node->key = strndup(env[i], key_len);
        new_node->value = strdup(equal_sign + 1);
        new_node->next = NULL;

        if (!head)
            head = new_node;
        else
            current->next = new_node;

        current = new_node;
    }

    return head;
}

static int env_size(t_env_node *env) {
    int i = 0;
    while (env) {
        i++;
        env = env->next;
    }
    return i;
}

t_env_list init_env_list(char **env) {
    t_env_list ret;
    t_env_node *envi = init_env(env);
    ret.head = envi;
    ret.size = env_size(envi);
    return ret;
}