#include "../include/minishell.h"

static int is_numeric_str(const char *s)
{
    int i = 0;
    
    // Skip quote markers at beginning
    while (s[i] && (s[i] == '\001' || s[i] == '\002' || s[i] == '\003' || s[i] == '\004'))
        i++;
    
    // Skip leading whitespace
    while (s[i] && ft_isspace(s[i]))
        i++;
    
    // Handle sign
    if (s[i] == '+' || s[i] == '-')
        i++;
    
    // Must have at least one digit
    if (!s[i] || !ft_isnum(s[i]))
        return 0;
    
    // Check all remaining characters are digits or quote markers
    while (s[i])
    {
        if (!ft_isnum(s[i]) && s[i] != '\001' && s[i] != '\002' && s[i] != '\003' && s[i] != '\004')
            return 0;
        i++;
    }
    
    return 1;
}

int builtin_exit(char **args)
{
    int exit_code = 0;
    
    printf("exit\n");

    if (args[1])
    {
        if (!is_numeric_str(args[1]))
        {
            ft_fprintf_stderr("minishell: exit: %s: numeric argument required\n", args[1]);
            exit(2);  // Bash returns 2 for non-numeric argument
        }
        
        if (args[2])
        {
            ft_fprintf_stderr("minishell: exit: too many arguments\n");
            return 1;
        }
        
        char *clean_arg = remove_quote_markers(args[1]);
        exit_code = ft_atoi(clean_arg);
        free(clean_arg);
        
        // Handle modulo 256 for exit code
        exit_code = exit_code % 256;
        if (exit_code < 0)
            exit_code += 256;
    }

    exit(exit_code);
}