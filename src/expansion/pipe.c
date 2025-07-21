#include "../include/minishell.h"

int has_pipe(t_cmd *cmds)
{
    return (cmds && cmds->next);
}

// Helper function to shift argv when first element is empty
static void shift_argv_pipeline(char ***argv)
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
int execute_pipeline(t_cmd *cmds, t_shell *shell)
{
    int pipefd[2];
    int input_fd = STDIN_FILENO;
    int status = 0;
    pid_t pid;
    pid_t last_pid = 0;
    t_cmd *current = cmds;

    while (current)
    {
        if (current->next && pipe(pipefd) == -1)
        {
            perror("pipe");
            return 1;
        }

        pid = fork();
        if (pid == 0)
        {
            // Child process
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);

            // Handle input redirections (including heredoc)
            int fd = -1;
            t_redir *redir;
            if (current->infiles) {
                for (redir = current->infiles; redir; redir = redir->next) {
                    if (redir->type == REDIR_INPUT) {
                        int tmp_fd = open(redir->filename, O_RDONLY);
                        if (tmp_fd == -1) {
                            perror(redir->filename);
                            if (fd != -1) close(fd);
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
            if (current->heredoc > 0) {
                dup2(current->heredoc, STDIN_FILENO);
                close(current->heredoc);
            } else if (input_fd != STDIN_FILENO) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            // Handle output redirections - only last one matters
            fd = -1;
            if (current->outfiles) {
                for (redir = current->outfiles; redir; redir = redir->next) {
                    int flags = O_WRONLY | O_CREAT;
                    if (redir->type == REDIR_APPEND)
                        flags |= O_APPEND;
                    else
                        flags |= O_TRUNC;
                    int tmp_fd = open(redir->filename, flags, 0644);
                    if (tmp_fd == -1) {
                        perror(redir->filename);
                        if (fd != -1) close(fd);
                        exit(1);
                    }
                    if (fd != -1) close(fd);
                    fd = tmp_fd;
                }
                if (fd != -1) {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            } else if (current->next) {
                dup2(pipefd[1], STDOUT_FILENO);
            }

            // Close all pipe ends
            if (current->next)
            {
                close(pipefd[0]);
                close(pipefd[1]);
            }

            // Handle empty expansion
            if (current->argv && current->argv[0] && current->argv[0][0] == '\0')
            {
                shift_argv_pipeline(&current->argv);
            }

            // Execute the command
            if (!current->argv || !current->argv[0] || current->argv[0][0] == '\0')
                exit(0);

            if (is_builtin(current->argv[0]))
            {
                exit(execute_builtin(current, shell));
            }
            else
            {
                exit(execute_external(current, shell));
            }
        }
        else if (pid > 0)
        {
            // Parent process
            if (input_fd != STDIN_FILENO) 
                close(input_fd);
            if (current->next)
            {
                close(pipefd[1]);
                input_fd = pipefd[0];
            }
            if (!current->next)
                last_pid = pid;
        }
        else
        {
            perror("fork");
            return 1;
        }
        
        current = current->next;
    }

    // Wait for all processes
    int temp_status;
    pid_t wpid;
    while ((wpid = waitpid(-1, &temp_status, 0)) > 0)
    {
        if (wpid == last_pid)
        {
            if (WIFEXITED(temp_status))
                status = WEXITSTATUS(temp_status);
            else if (WIFSIGNALED(temp_status))
                status = 128 + WTERMSIG(temp_status);
        }
    }
    
    return status;
}