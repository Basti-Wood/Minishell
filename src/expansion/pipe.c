#include "../../include/minishell.h"

int has_pipe(t_cmd *cmds)
{
    if (!cmds)
        return 0;
    
    // Check if there's more than one command (connected by pipes)
    return (cmds->next != NULL);
}

int execute_pipeline(t_cmd *cmds, t_shell *shell)
{
    int **pipes;
    pid_t *pids;
    int cmd_count = 0;
    int status = 0;
    int i;
    t_cmd *current;
    
    // Count commands
    current = cmds;
    while (current) {
        cmd_count++;
        current = current->next;
    }
    
    if (cmd_count < 2)
        return execute_command(cmds, shell);
    
    // Allocate pipe arrays
    pipes = malloc(sizeof(int *) * (cmd_count - 1));
    pids = malloc(sizeof(pid_t) * cmd_count);
    
    if (!pipes || !pids) {
        perror("malloc");
        free(pipes);
        free(pids);
        return 1;
    }
    
    // Create pipes
    for (i = 0; i < cmd_count - 1; i++) {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i] || pipe(pipes[i]) == -1) {
            perror("pipe");
            // Clean up
            for (int j = 0; j <= i; j++) {
                if (pipes[j]) {
                    if (j < i) {
                        close(pipes[j][0]);
                        close(pipes[j][1]);
                    }
                    free(pipes[j]);
                }
            }
            free(pipes);
            free(pids);
            return 1;
        }
    }
    
    // Execute commands
    current = cmds;
    for (i = 0; i < cmd_count; i++) {
        pids[i] = fork();
        
        if (pids[i] == -1) {
            perror("fork");
            status = 1;
            break;
        }
        
        if (pids[i] == 0) {
            // Child process
            
            // Reset signal handlers
            signal(SIGPIPE, SIG_DFL);
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            
            // Set up pipes
            if (i > 0) {
                // Not first command - redirect stdin from previous pipe
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            
            if (i < cmd_count - 1) {
                // Not last command - redirect stdout to next pipe
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            // Close all pipe file descriptors
            for (int j = 0; j < cmd_count - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            // Handle redirections for this command
            if (current->infiles || current->outfiles || current->heredoc > 0) {
                // Handle input redirections
                if (current->infiles) {
                    int fd = -1;
                    t_redir *redir;
                    for (redir = current->infiles; redir; redir = redir->next) {
                        if (redir->type == REDIR_INPUT) {
                            int tmp_fd = open(redir->filename, O_RDONLY);
                            if (tmp_fd == -1) {
                                perror(redir->filename);
                                exit(1);
                            }
                            if (fd != -1) close(fd);
                            fd = tmp_fd;
                        }
                    }
                    if (fd != -1) {
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                    }
                }
                
                // Handle heredoc
                if (current->heredoc > 0) {
                    dup2(current->heredoc, STDIN_FILENO);
                    close(current->heredoc);
                }
                
                // Handle output redirections
                if (current->outfiles) {
                    int fd = -1;
                    t_redir *redir;
                    for (redir = current->outfiles; redir; redir = redir->next) {
                        int flags = O_WRONLY | O_CREAT;
                        if (redir->type == REDIR_APPEND)
                            flags |= O_APPEND;
                        else
                            flags |= O_TRUNC;
                        int tmp_fd = open(redir->filename, flags, 0644);
                        if (tmp_fd == -1) {
                            perror(redir->filename);
                            exit(1);
                        }
                        if (fd != -1) close(fd);
                        fd = tmp_fd;
                    }
                    if (fd != -1) {
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }
                }
            }
            
            // Execute the command
            if (current->argv && current->argv[0]) {
                if (is_builtin(current->argv[0])) {
                    exit(execute_builtin(current, shell));
                } else {
                    // For external commands, we need to handle them properly
                    char *executable = find_executable(current->argv[0], shell->envp);
                    if (!executable) {
                        fprintf(stderr, "minishell: %s: command not found\n", current->argv[0]);
                        exit(127);
                    }
                    
                    char **env_array = env_list_to_array(shell->envp);
                    if (!env_array) {
                        free(executable);
                        exit(1);
                    }
                    
                    execve(executable, current->argv, env_array);
                    perror("execve");
                    exit(126);
                }
            }
            exit(0);
        }
        
        current = current->next;
    }
    
    // Parent process - close all pipes
    for (i = 0; i < cmd_count - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    // Wait for all children
    for (i = 0; i < cmd_count; i++) {
        if (pids[i] > 0) {
            int child_status;
            waitpid(pids[i], &child_status, 0);
            // Keep the exit status of the last command
            if (i == cmd_count - 1) {
                if (WIFEXITED(child_status))
                    status = WEXITSTATUS(child_status);
                else if (WIFSIGNALED(child_status))
                    status = 128 + WTERMSIG(child_status);
            }
        }
    }
    
    // Clean up
    for (i = 0; i < cmd_count - 1; i++) {
        free(pipes[i]);
    }
    free(pipes);
    free(pids);
    
    return status;
}