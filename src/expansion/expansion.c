#include "../include/minishell.h"

char *expand_variables(char *str, t_shell *shell)
{
    char *result = malloc(1024); // Should be dynamic
    int i = 0, j = 0;
    char *var_name;
    char *var_value;
    
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1])
        {
            i++; // Skip $
            
            // Handle $?
            if (str[i] == '?')
            {
                char *exit_str = ft_itoa(shell->exit_status);
                ft_strcpy(result + j, exit_str);
                j += ft_strlen(exit_str);
                free(exit_str);
                i++;
            }
            else if (ft_isalpha(str[i]) || str[i] == '_')
            {
                // Extract variable name
                int start = i;
                while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                    i++;
                
                var_name = ft_substr(str, start, i - start);
                var_value = get_env_value(shell->envp, var_name);
                
                if (var_value)
                {
                    ft_strcpy(result + j, var_value);
                    j += ft_strlen(var_value);
                }
                
                free(var_name);
            }
            else
            {
                result[j++] = '$';
                result[j++] = str[i++];
            }
        }
        else
        {
            result[j++] = str[i++];
        }
    }
    
    result[j] = '\0';
    return result;
}