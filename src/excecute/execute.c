// execute.c
#include "../include/minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return 0;
    if (ft_strcmp(cmd, "echo") == 0)
        return 1;
    if (ft_strcmp(cmd, "cd") == 0)
        return 1;
    if (ft_strcmp(cmd, "pwd") == 0)
        return 1;
    if (ft_strcmp(cmd, "export") == 0)
        return 1;
    if (ft_strcmp(cmd, "unset") == 0)
        return 1;
    if (ft_strcmp(cmd, "env") == 0)
        return 1;
    if (ft_strcmp(cmd, "exit") == 0)
        return 1;
    return 0;
}

int execute_builtin(t_cmd *cmd, t_shell *shell)
{
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return 1;
        
    if (ft_strcmp(cmd->argv[0], "echo") == 0)
        return builtin_echo(cmd->argv);
    if (ft_strcmp(cmd->argv[0], "cd") == 0)
        return builtin_cd(cmd->argv, shell->envp);
    if (ft_strcmp(cmd->argv[0], "pwd") == 0)
        return builtin_pwd();
    if (ft_strcmp(cmd->argv[0], "env") == 0)
        return builtin_env(shell->envp);
    if (ft_strcmp(cmd->argv[0], "export") == 0)
        return builtin_export(cmd->argv, shell->envp);
    if (ft_strcmp(cmd->argv[0], "unset") == 0)
        return builtin_unset(cmd->argv, shell->envp);
    if (ft_strcmp(cmd->argv[0], "exit") == 0)
        return builtin_exit(cmd->argv);
    return 1;
}

char *find_executable(char *cmd, t_env_list *env_list)
{
    char *path_env;
    char **paths;
    char *full_path;
    int i;
    
    if (!cmd || !env_list)
        return NULL;
    
    // If command contains '/', it's a path
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        return NULL;
    }
    
    // Get PATH environment variable
    path_env = get_env_value(env_list, "PATH");
    if (!path_env)
        return NULL;
    
    paths = ft_split(path_env, ':');
    if (!paths)
        return NULL;
    
    i = 0;
    while (paths[i])
    {
        size_t path_len = ft_strlen(paths[i]);
        size_t cmd_len = ft_strlen(cmd);
        full_path = malloc(path_len + cmd_len + 2);
        if (!full_path)
        {
            // Free paths array
            int j = 0;
            while (paths[j])
                free(paths[j++]);
            free(paths);
            return NULL;
        }
        
        ft_strcpy(full_path, paths[i]);
        ft_strcat(full_path, "/");
        ft_strcat(full_path, cmd);
        
        if (access(full_path, X_OK) == 0)
        {
            // Free paths array
            int j = 0;
            while (paths[j])
                free(paths[j++]);
            free(paths);
            return full_path;
        }
        
        free(full_path);
        i++;
    }
    
    // Free paths array
    i = 0;
    while (paths[i])
        free(paths[i++]);
    free(paths);
    
    return NULL;
}

char **env_list_to_array(t_env_list *env_list)
{
    if (!env_list || !env_list->head)
        return NULL;
        
    char **env_array = malloc(sizeof(char *) * (env_list->size + 1));
    if (!env_array)
        return NULL;
        
    t_env_node *current = env_list->head;
    int i = 0;
    
    while (current && i < env_list->size)
    {
        if (!current->key || !current->value)
        {
            current = current->next;
            continue;
        }
        
        size_t key_len = ft_strlen(current->key);
        size_t value_len = ft_strlen(current->value);
        env_array[i] = malloc(key_len + value_len + 2);
        if (!env_array[i])
        {
            // Free previously allocated strings
            for (int j = 0; j < i; j++)
                free(env_array[j]);
            free(env_array);
            return NULL;
        }
        
        ft_strcpy(env_array[i], current->key);
        ft_strcat(env_array[i], "=");
        ft_strcat(env_array[i], current->value);
        current = current->next;
        i++;
    }
    env_array[i] = NULL;
    
    return env_array;
}

int execute_external(t_cmd *cmd, t_shell *shell)
{
    char *executable;
    char **env_array;
    pid_t pid;
    int status;
    
    if (!cmd || !cmd->argv || !cmd->argv[0] || !shell)
        return 1;
    
    executable = find_executable(cmd->argv[0], shell->envp);
    if (!executable)
    {
        fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
        return 127;
    }
    
    env_array = env_list_to_array(shell->envp);
    if (!env_array)
    {
        free(executable);
        return 1;
    }
    
    pid = fork();
    if (pid == 0)
    {
        // Child process
        // Handle input/output redirection
        if (cmd->infile)
        {
            int fd = open(cmd->infile, O_RDONLY);
            if (fd == -1)
            {
                perror(cmd->infile);
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        
        if (cmd->outfile)
        {
            int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
            int fd = open(cmd->outfile, flags, 0644);
            if (fd == -1)
            {
                perror(cmd->outfile);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        
        if (cmd->heredoc > 0)
        {
            dup2(cmd->heredoc, STDIN_FILENO);
            close(cmd->heredoc);
        }
        
        if (execve(executable, cmd->argv, env_array) == -1)
        {
            perror("execve");
            exit(126);
        }
    }
    else if (pid > 0)
    {
        // Parent process
        waitpid(pid, &status, 0);
        free(executable);
        
        // Free env_array
        int i = 0;
        while (env_array[i])
            free(env_array[i++]);
        free(env_array);
        
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            return 128 + WTERMSIG(status);
        else
            return 1;
    }
    else
    {
        perror("fork");
        free(executable);
        // Free env_array
        if (env_array)
        {
            int i = 0;
            while (env_array[i])
                free(env_array[i++]);
            free(env_array);
        }
        return 1;
    }
    
    return 1;
}

int execute_command(t_cmd *cmd, t_shell *shell)
{
    if (!cmd || !cmd->argv || !cmd->argv[0] || !shell)
        return 0;
    
    if (is_builtin(cmd->argv[0]))
        return execute_builtin(cmd, shell);
    else
        return execute_external(cmd, shell);
}