#include "../include/minishell.h"

int builtin_cd(char **args, t_env_list *env_list)
{
    char *path;
    char *home;
    
    if (!args[1])
    {
        // No argument - go to HOME
        home = get_env_value(env_list, "HOME");
        if (!home)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
        path = home;
    }
    else
    {
        path = args[1];
    }
    
    if (chdir(path) != 0)
    {
        perror("cd");
        return 1;
    }
    
    // Update PWD environment variable
    char *new_pwd = getcwd(NULL, 0);
    if (new_pwd)
    {
        set_env_value(env_list, "PWD", new_pwd);
        free(new_pwd);
    }
    
    return 0;
}