#include "../include/minishell.h"
/*
char *remove_quote_markers(const char *str)
{
    char *result = malloc(ft_strlen(str) + 1);
    int j = 0;
    
    for (int i = 0; str[i]; i++)
    {
        // Skip quote markers
        if (str[i] != '\001' && str[i] != '\002' && str[i] != '\003' && str[i] != '\004')
            result[j++] = str[i];
    }
    result[j] = '\0';
    return result;
}
*/
t_token *handle_empty_expansions(t_token *tokens)
{
    t_token *head = tokens;
    t_token *current = tokens;
    t_token *prev = NULL;
    
    while (current)
    {
        // Check if this token is empty and it's supposed to be a command
        if (current->type == CMD && current->str && current->str[0] == '\0')
        {
            t_token *to_remove = current;
            
            // If there's a next token, it becomes the command
            if (current->next)
            {
                current->next->type = CMD;
                current = current->next;
                
                // Fix the linked list
                if (prev)
                    prev->next = current;
                else
                    head = current;
                    
                current->prev = prev;
                
                // Free the empty token
                free(to_remove->str);
                free(to_remove);
                continue;
            }
            else
            {
                // No more tokens, just remove this one
                if (prev)
                    prev->next = NULL;
                else
                    head = NULL;
                    
                free(to_remove->str);
                free(to_remove);
                break;
            }
        }
        
        prev = current;
        current = current->next;
    }
    
    return head;
}

static void append_char(char **str, char c, int *len, int *capacity)
{
    if (*len + 2 >= *capacity) // +2 for char and null-terminator
    {
        *capacity *= 2;
        *str = realloc(*str, *capacity);
    }
    (*str)[(*len)++] = c;
    (*str)[*len] = '\0';
}

static void append_string(char **result, const char *str_to_append, int *len, int *capacity)
{
    int i = 0;
    while (str_to_append[i])
    {
        append_char(result, str_to_append[i], len, capacity);
        i++;
    }
}

char *expand_variables(char *str, t_shell *shell)
{
    int capacity = 128;
    int len = 0;
    char *result = malloc(capacity);
    if (!result)
        return NULL;
    result[0] = '\0';
    
    int i = 0;
    int in_single_quotes = 0;

      while (str[i])
    {
        // Handle quote markers - DON'T append them to result!
        if (str[i] == '\001')  // Start single quote marker
        {
            in_single_quotes = 1;
            i++;
            continue;
        }
        else if (str[i] == '\003')  // End single quote marker
        {
            in_single_quotes = 0;
            i++;
            continue;
        }
        else if (str[i] == '\002')  // Start double quote marker
        {
            i++;
            continue;
        }
        else if (str[i] == '\004')  // End double quote marker
        {
            i++;
            continue;
        }

        // Handle variable expansion (not in single quotes)
        if (str[i] == '$' && !in_single_quotes)
        {
            i++; // Skip '$'
            
            if (!str[i])
            {
                // Just a dollar sign at end
                append_char(&result, '$', &len, &capacity);
            }
            else if (str[i] == '?')
            {
                char *exit_str = ft_itoa(shell->exit_status);
                append_string(&result, exit_str, &len, &capacity);
                free(exit_str);
                i++;
            }
            else if (ft_isalpha(str[i]) || str[i] == '_')
            {
                int start = i;
                while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                    i++;
                char *var_name = ft_substr(str, start, i - start);
                char *var_value = get_env_value(shell->envp, var_name);
                if (var_value)
                    append_string(&result, var_value, &len, &capacity);
                else
                    append_string(&result, "", &len, &capacity);
                free(var_name);
            }
            else if (ft_isnum(str[i]))
            {
                // Skip positional parameters (not supported)
                i++;
            }
            else
            {
                // Just a dollar sign followed by non-variable character, treat as literal
                append_char(&result, '$', &len, &capacity);
                // Do not increment i here, so the next character is processed as normal
            }
        }
        else
        {
            append_char(&result, str[i], &len, &capacity);
            i++;
        }
    }
    
    result[len] = '\0';
    return result;
}
