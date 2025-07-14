#include "../include/minishell.h"

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    
    minishell(env);  // Pass environment to minishell
    return 0;
}