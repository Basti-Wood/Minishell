#include "../include/minishell.h"

int builtin_cd(char **args, t_env_list *env_list)
{
    char *path;
    char *home;
    char *old_pwd;
    char current_pwd[1024];

	if (args[1] && args[2])
    {
        ft_fprintf_stderr("minishell: cd: too many arguments\n");
        return 1;
    }

    if (getcwd(current_pwd, sizeof(current_pwd)) == NULL)
    {
        perror("cd: getcwd error");
        return 1;
    }
    old_pwd = get_env_value(env_list, "PWD");

    if (!args[1])
    {
        home = get_env_value(env_list, "HOME");
        if (!home)
        {
            ft_fprintf_stderr("cd: HOME not set\n");
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

    if (old_pwd)
        set_env_value(env_list, "OLDPWD", old_pwd);
    
    char *new_pwd = getcwd(NULL, 0);
    if (new_pwd)
    {
        set_env_value(env_list, "PWD", new_pwd);
        free(new_pwd);
    }
    
    return 0;
}