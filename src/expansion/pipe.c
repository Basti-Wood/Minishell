#include "../../include/minishell.h"

int has_pipe(t_cmd *cmds)
{
    if (!cmds)
        return 0;
    
    // Check if there's more than one command (connected by pipes)
    return (cmds->next != NULL);
}

// Handle redirections in the order they appear in the command line
int handle_redirections_in_order(t_cmd *cmd)
{
    // Only process if we have ordered redirections
    if (!cmd->redirs) {
        // Handle heredoc if present (for backward compatibility)
        if (cmd->heredoc > 0) {
            dup2(cmd->heredoc, STDIN_FILENO);
            close(cmd->heredoc);
        }
        return 0;
    }
    
    // NEW: Process redirections sequentially, stopping on first failure
    t_redir *redir;
    for (redir = cmd->redirs; redir; redir = redir->next) {
        if (redir->type == REDIR_INPUT) {
            // Check if input file exists before trying to open it
            if (access(redir->filename, F_OK) == -1) {
                ft_fprintf_stderr("minishell: %s: No such file or directory\n", redir->filename);
                return -1; // Stop processing on failure
            }
            if (access(redir->filename, R_OK) == -1) {
                ft_fprintf_stderr("minishell: %s: Permission denied\n", redir->filename);
                return -1;
            }
            
            // Open and apply input redirection
            int fd = open(redir->filename, O_RDONLY);
            if (fd == -1) {
                perror(redir->filename);
                return -1;
            }
            
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND) {
            // Create/open output file (succeeds even if later redirections fail)
            int flags = O_WRONLY | O_CREAT;
            if (redir->type == REDIR_APPEND)
                flags |= O_APPEND;
            else
                flags |= O_TRUNC;
                
            int fd = open(redir->filename, flags, 0644);
            if (fd == -1) {
                perror(redir->filename);
                return -1;
            }
            
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == REDIR_HEREDOC) {
            // Handle heredoc
            if (cmd->heredoc > 0) {
                dup2(cmd->heredoc, STDIN_FILENO);
                close(cmd->heredoc);
            }
        }
    }
    
    return 0;
}

// Modified pipeline execution - replace the redirection handling section
int execute_pipeline(t_cmd *cmds, t_shell *shell)
{
    int **pipes;
    pid_t *pids;
    int cmd_count = 0;
    int status = 0;
    int i;
    t_cmd *current;
    int broken_pipe_detected = 0;
    
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
            
            // Reset signal handlers - let SIGPIPE kill the process naturally
            signal(SIGPIPE, SIG_DFL);
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            
            // Set up pipes FIRST
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
            
            // Handle redirections in correct order - this will override pipe setup if needed
            if (current->redirs || current->infiles || current->outfiles || current->heredoc > 0) {
                if (handle_redirections_in_order(current) == -1) {
                    exit(1); // Exit with error if redirection fails
                }
            }
            
            // Execute the command
            if (current->argv && current->argv[0] && current->argv[0][0] != '\0') {
                if (is_builtin(current->argv[0])) {
                    exit(execute_builtin(current, shell));
                } else {
                    // For external commands
                    char *executable = find_executable(current->argv[0], shell->envp);
                    if (!executable) {
                        ft_fprintf_stderr("minishell: %s: command not found\n", current->argv[0]);
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
    
    // Wait for all children and check for broken pipes
    int *child_statuses = malloc(sizeof(int) * cmd_count);
    int last_command_failed = 0;
    
    for (i = 0; i < cmd_count; i++) {
        if (pids[i] > 0) {
            waitpid(pids[i], &child_statuses[i], 0);
            
            // Keep the exit status of the last command
            if (i == cmd_count - 1) {
                if (WIFEXITED(child_statuses[i])) {
                    status = WEXITSTATUS(child_statuses[i]);
                    if (status != 0) {
                        last_command_failed = 1;
                    }
                } else if (WIFSIGNALED(child_statuses[i])) {
                    int sig = WTERMSIG(child_statuses[i]);
                    if (sig == SIGPIPE)
                        status = 0;  // Broken pipe is not an error for the last command
                    else
                        status = 128 + sig;
                }
            }
        }
    }
    
    // Now check for broken pipes, but only report if last command didn't fail
    if (!last_command_failed) {
        for (i = 0; i < cmd_count - 1; i++) {
            if (WIFSIGNALED(child_statuses[i]) && WTERMSIG(child_statuses[i]) == SIGPIPE) {
                broken_pipe_detected = 1;
                break;
            }
        }
    }
    
    free(child_statuses);
    
    // Print "Broken pipe" message if detected (like bash does)
    if (broken_pipe_detected) {
        ft_fprintf_stderr(" Broken pipe\n");
    }
    
    // Clean up
    for (i = 0; i < cmd_count - 1; i++) {
        free(pipes[i]);
    }
    free(pipes);
    free(pids);
    
    return status;
}