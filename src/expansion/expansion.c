#include "../include/minishell.h"

static void append_char(char **str, char c, int *len, int *capacity)
{
    if (*len + 1 >= *capacity)
    {
        *capacity *= 2;
        *str = realloc(*str, *capacity);
    }
    (*str)[(*len)++] = c;
}

static void append_string(char **result, const char *str_to_append, int *len, int *capacity)
{
    int append_len = ft_strlen(str_to_append);
    while (*len + append_len >= *capacity)
    {
        *capacity *= 2;
        *result = realloc(*result, *capacity);
    }
    ft_strlcat(*result, str_to_append, *capacity);
    *len += append_len;
}

char *expand_variables(char *str, t_shell *shell)
{
    int capacity = 128;
    int len = 0;
    char *result = malloc(capacity);
    result[0] = '\0';
    
    int i = 0;
    char in_quote = 0;

    while (str[i])
    {

        if ((str[i] == '\'' || str[i] == '"') && !in_quote)
        {
            in_quote = str[i];
            i++;
            continue;
        }
        if (str[i] == in_quote)
        {
            in_quote = 0;
            i++;
            continue;
        }

        if (str[i] == '$' && in_quote != '\'')
        {
            i++; // Skip '$'
            if (str[i] == '?')
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
                free(var_name);
            }
            else
            {
                append_char(&result, '$', &len, &capacity);
            }
        }
        else
        {
            append_char(&result, str[i], &len, &capacity);
            i++;
        }
    }
    result[len] = '\0';
    return (result);
}