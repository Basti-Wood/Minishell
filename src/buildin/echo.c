#include "../include/minishell.h"

int builtin_echo(char **args)
{
    int i = 1;
    int newline = 1;

    if (args[1] && ft_strcmp(args[1], "-n") == 0)
    {
        newline = 0;
        i = 2;
    }

    while (args[i])
    {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
    
    if (newline)
        ft_putstr_fd("\n", 1);
    
    return 0;
}