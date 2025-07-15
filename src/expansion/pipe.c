#include "../include/minishell.h"
int execute_pipeline(t_cmd *cmds, t_shell *shell) {
    int pipefd[2];
    int input_fd = STDIN_FILENO;
    int status = 0;
    pid_t pid;
    t_cmd *current = cmds;

    while (current) {
        if (current->next && pipe(pipefd) == -1) {
            perror("pipe");
            return 1;
        }

        pid = fork();
        if (pid == 0) {
            // Child process
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            
            // Handle input redirection (fixed order)
            if (current->heredoc > 0) {
                dup2(current->heredoc, STDIN_FILENO);
                close(current->heredoc);
            }
            else if (current->infile) {
                int fd = open(current->infile, O_RDONLY);
                if (fd == -1) {
                    perror(current->infile);
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (input_fd != STDIN_FILENO) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            // Handle output redirection
            if (current->outfile) {
                int flags = O_WRONLY | O_CREAT | (current->append ? O_APPEND : O_TRUNC);
                int fd = open(current->outfile, flags, 0644);
                if (fd == -1) {
                    perror(current->outfile);
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (current->next) {
                dup2(pipefd[1], STDOUT_FILENO);
            }
            
            // Close all pipe ends
            if (current->next) {
                close(pipefd[0]);
                close(pipefd[1]);
            }
            
            // Execute the command
            if (is_builtin(current->argv[0])) {
                exit(execute_builtin(current, shell));
            } else {
                execute_external(current, shell);
                exit(shell->exit_status);
            }
        }
        else if (pid > 0) {
            // Parent process
            if (input_fd != STDIN_FILENO) 
                close(input_fd);
            if (current->next) {
                close(pipefd[1]);
                input_fd = pipefd[0];
            }
        }
        else {
            perror("fork");
            return 1;
        }
        
        current = current->next;
    }

    // Wait for all processes and get exit status from last one
    while (waitpid(-1, &status, 0) > 0) {
        if (WIFEXITED(status)) {
            shell->exit_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            shell->exit_status = 128 + WTERMSIG(status);
        }
    }
    
    return shell->exit_status;
}