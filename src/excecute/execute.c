#include "../include/minishell.h"
#include <errno.h>

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
    struct stat st;
    
    if (!cmd || !env_list)
        return NULL;
        
    // Handle absolute or relative paths
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, F_OK) == 0)
        {
            if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
                return NULL;
            return ft_strdup(cmd);
        }
        return NULL;
    }
    
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
            int j = 0;
            while (paths[j])
                free(paths[j++]);
            free(paths);
            return full_path;
        }
        
        free(full_path);
        i++;
    }

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

// Helper function to shift argv when first element is empty
static void shift_argv(char ***argv)
{
    int i = 0;
    
    // Find first non-empty argument
    while ((*argv)[i] && (*argv)[i][0] == '\0')
    {
        free((*argv)[i]);
        i++;
    }
    
    if (!(*argv)[i])
    {
        // All arguments are empty
        free((*argv)[0]);
        (*argv)[0] = NULL;
        return;
    }
    
    // Shift remaining arguments
    int j = 0;
    while ((*argv)[i])
    {
        (*argv)[j] = (*argv)[i];
        j++;
        i++;
    }
    (*argv)[j] = NULL;
}

int execute_external(t_cmd *cmd, t_shell *shell)
{
    char *executable;
    char **env_array;
    pid_t pid;
    int status;
    struct stat st;
    //int redirection_error = 0;
    
    if (!cmd || !cmd->argv || !cmd->argv[0] || !shell)
        return 1;
    
    // Handle empty command (from empty variable expansion)
    if (cmd->argv[0][0] == '\0')
    {
        shift_argv(&cmd->argv);
        if (!cmd->argv[0])
            return 0;
    }
    
    // Check for directory
    if (ft_strchr(cmd->argv[0], '/'))
    {
        if (stat(cmd->argv[0], &st) == 0 && S_ISDIR(st.st_mode))
        {
            ft_fprintf_stderr("minishell: %s: Is a directory\n", cmd->argv[0]);
            return 126;
        }
        else if (access(cmd->argv[0], F_OK) != 0)
        {
            ft_fprintf_stderr("minishell: %s: No such file or directory\n", cmd->argv[0]);
            return 127;
        }
        else if (access(cmd->argv[0], X_OK) != 0)
        {
            ft_fprintf_stderr("minishell: %s: Permission denied\n", cmd->argv[0]);
            return 126;
        }
    }
    
    executable = find_executable(cmd->argv[0], shell->envp);
    if (!executable)
    {
        ft_fprintf_stderr("minishell: %s: command not found\n", cmd->argv[0]);
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
        // Note: When called from execute_with_redirections, redirections
        // are already set up. When called from execute_pipeline, we don't
        // need to set them up here as they're handled in the pipeline function.
        
        if (execve(executable, cmd->argv, env_array) == -1)
        {
            // Check if file is not executable
            if (errno == ENOEXEC)
            {
                // Try to execute as shell script
                char *sh_argv[4];
                sh_argv[0] = "/bin/sh";
                sh_argv[1] = executable;
                sh_argv[2] = NULL;
                
                // Copy remaining arguments
                if (cmd->argv[1])
                {
                    int argc = 0;
                    while (cmd->argv[argc])
                        argc++;
                    
                    char **new_argv = malloc(sizeof(char *) * (argc + 2));
                    new_argv[0] = "/bin/sh";
                    new_argv[1] = executable;
                    for (int j = 1; j < argc; j++)
                        new_argv[j + 1] = cmd->argv[j];
                    new_argv[argc + 1] = NULL;
                    
                    execve("/bin/sh", new_argv, env_array);
                    free(new_argv);
                }
                else
                {
                    execve("/bin/sh", sh_argv, env_array);
                }
                
                // If still fails, exit with 126
                ft_fprintf_stderr("minishell: %s: Exec format error\n", executable);
                exit(126);
            }
            perror("execve");
            exit(126);
        }
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        free(executable);
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

// Execute commands with redirections (used by pipelines)
// Process redirections in exact order, stopping on first failure
static int execute_with_redirections(t_cmd *cmd, t_shell *shell)
{
    int stdin_backup = -1;
    int stdout_backup = -1;
    int status = 0;
    int has_redirection_error = 0;

    // NEW: Process redirections in exact order (left to right)
    // This matches bash behavior: earlier redirections succeed even if later ones fail
    if (cmd->redirs) {
        t_redir *redir;
        for (redir = cmd->redirs; redir; redir = redir->next) {
            if (redir->type == REDIR_INPUT) {
                // Check if input file exists before trying to open it
                if (access(redir->filename, F_OK) == -1) {
                    ft_fprintf_stderr("minishell: %s: No such file or directory\n", redir->filename);
                    has_redirection_error = 1;
                    break; // Stop processing further redirections
                }
                if (access(redir->filename, R_OK) == -1) {
                    ft_fprintf_stderr("minishell: %s: Permission denied\n", redir->filename);
                    has_redirection_error = 1;
                    break;
                }
                
                // Open input file
                int fd = open(redir->filename, O_RDONLY);
                if (fd == -1) {
                    perror(redir->filename);
                    has_redirection_error = 1;
                    break;
                }
                
                // Apply input redirection
                if (stdin_backup == -1)
                    stdin_backup = dup(STDIN_FILENO);
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND) {
                // Create/open output file (this succeeds even if later redirections fail)
                int flags = O_WRONLY | O_CREAT;
                if (redir->type == REDIR_APPEND)
                    flags |= O_APPEND;
                else
                    flags |= O_TRUNC;
                    
                int fd = open(redir->filename, flags, 0644);
                if (fd == -1) {
                    perror(redir->filename);
                    has_redirection_error = 1;
                    break;
                }
                
                // Apply output redirection
                if (stdout_backup == -1)
                    stdout_backup = dup(STDOUT_FILENO);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
        }
    }
    
    // Fallback to old logic if no ordered redirections
    if (!cmd->redirs) {
        // Process output redirections first (original logic)
        if (cmd->outfiles) {
            t_redir *redir;
            int fd = -1;
            for (redir = cmd->outfiles; redir; redir = redir->next) {
                int flags = O_WRONLY | O_CREAT;
                if (redir->type == REDIR_APPEND)
                    flags |= O_APPEND;
                else
                    flags |= O_TRUNC;
                int tmp_fd = open(redir->filename, flags, 0644);
                if (tmp_fd == -1) {
                    perror(redir->filename);
                    if (fd != -1) close(fd);
                    has_redirection_error = 1;
                    status = 1;
                    break;
                }
                if (fd != -1) close(fd);
                fd = tmp_fd;
            }
            if (fd != -1 && !has_redirection_error) {
                stdout_backup = dup(STDOUT_FILENO);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (fd != -1) {
                close(fd);
            }
        }

        // Process input redirections
        if (cmd->infiles && !has_redirection_error) {
            t_redir *redir;
            int fd = -1;
            for (redir = cmd->infiles; redir; redir = redir->next) {
                if (redir->type == REDIR_INPUT) {
                    int tmp_fd = open(redir->filename, O_RDONLY);
                    if (tmp_fd == -1) {
                        if (errno == ENOENT)
                            ft_fprintf_stderr("minishell: %s: No such file or directory\n", redir->filename);
                        else if (errno == EACCES)
                            ft_fprintf_stderr("minishell: %s: Permission denied\n", redir->filename);
                        else
                            perror(redir->filename);
                        if (fd != -1) close(fd);
                        has_redirection_error = 1;
                        status = 1;
                        break;
                    }
                    if (fd != -1) close(fd);
                    fd = tmp_fd;
                }
            }
            if (fd != -1 && !has_redirection_error) {
                stdin_backup = dup(STDIN_FILENO);
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (fd != -1) {
                close(fd);
            }
        }
    }
    
    // Handle heredoc as highest priority input (only if no errors)
    if (cmd->heredoc > 0 && !has_redirection_error) {
        if (stdin_backup == -1)
            stdin_backup = dup(STDIN_FILENO);
        dup2(cmd->heredoc, STDIN_FILENO);
        close(cmd->heredoc);
    }
    else if (cmd->heredoc > 0) {
        close(cmd->heredoc);
    }

    // Execute command only if there were no redirection errors
    if (!has_redirection_error && cmd->argv && cmd->argv[0] && cmd->argv[0][0] != '\0') {
        if (is_builtin(cmd->argv[0]))
            status = execute_builtin(cmd, shell);
        else
            status = execute_external(cmd, shell);
    } else if (has_redirection_error) {
        status = 1;
    }

    // Reset file descriptors
    if (stdin_backup != -1) {
        dup2(stdin_backup, STDIN_FILENO);
        close(stdin_backup);
    }
    if (stdout_backup != -1) {
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdout_backup);
    }
    return status;
}


int execute_command(t_cmd *cmd, t_shell *shell)
{
    if (!cmd)
        return 0;
    
    // Handle empty command (from empty variable expansion)
    if (cmd->argv && cmd->argv[0] && cmd->argv[0][0] == '\0')
    {
        shift_argv(&cmd->argv);
        if (!cmd->argv[0])
            return 0;
    }
    
    // Use the common execution function
    return execute_with_redirections(cmd, shell);
}