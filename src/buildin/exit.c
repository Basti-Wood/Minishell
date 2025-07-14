#include "../include/minishell.h"

// Helper to check if a string is composed only of digits
static int is_numeric(const char *s)
{
    if (*s == '+' || *s == '-')
        s++;
    if (!*s)
        return 0;
    while (*s)
    {
        if (!ft_isnum(*s))
            return 0;
        s++;
    }
    return 1;
}

int builtin_exit(char **args)
{
    int exit_code = 0;
    
    printf("exit\n");

    if (args[1])
    {
        if (!is_numeric(args[1]))
        {
            fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
            exit(255);
        }
        if (args[2])
        {
            fprintf(stderr, "minishell: exit: too many arguments\n");
            return 1; // Don't exit, just return an error
        }
        exit_code = ft_atoi(args[1]);
    }
    
    // Perform cleanup of your shell's resources here before exiting
    // e.g., free_env_list(shell->envp);
    
    exit(exit_code % 256);
}
