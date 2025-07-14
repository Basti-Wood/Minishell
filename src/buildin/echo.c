#include "../include/minishell.h"

int builtin_echo(char **args)
{
    int i = 1;
    bool newline = true;
    
    // Check for -n flag
    if (args[1] && ft_strcmp(args[1], "-n") == 0)
    {
        newline = false;
        i = 2;
    }
    
    // Print arguments
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    
    if (newline)
        printf("\n");
    
    return 0;
}