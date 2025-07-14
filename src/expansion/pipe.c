#include "../include/minishell.h"

int execute_pipeline(t_cmd *cmds, t_shell *shell) {
    int pipefd[2];
    int input_fd = STDIN_FILENO;
    int status = 0;
    pid_t pid;
    t_cmd *current = cmds;
    int last_pid = -1;

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
            
            // Handle input redirection
            if (current->infile) {
                int fd = open(current->infile, O_RDONLY);
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
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (current->next) {
                dup2(pipefd[1], STDOUT_FILENO);
            }
            if (current->heredoc) {
				dup2(current->heredoc, STDIN_FILENO);
				close(current->heredoc);
			}
            // Close all pipe ends
            close(pipefd[0]);
            close(pipefd[1]);
            
            execute_command(current, shell);
            exit(shell->exit_status);
        }
        else if (pid > 0) {
            // Parent process
            if (input_fd != STDIN_FILENO) close(input_fd);
            if (current->next) {
                close(pipefd[1]);
                input_fd = pipefd[0];
            }
            last_pid = pid;
        }
        current = current->next;
    }

    // Wait for last process in pipeline
    if (last_pid != -1) {
        waitpid(last_pid, &status, 0);
        shell->exit_status = WEXITSTATUS(status);
    }
    return shell->exit_status;
}