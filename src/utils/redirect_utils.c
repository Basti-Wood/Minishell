#include "../include/minishell.h"

int redirect_input(t_cmd *cmd)
{
    t_redir *redir = NULL;
    int fd = -1;
    // Use the last input redirection (as bash does)
    for (redir = cmd->infiles; redir; redir = redir->next) {
        if (redir->type == REDIR_INPUT) {
            if (access(redir->filename, F_OK) == -1) {
                fprintf(stderr, "minishell: %s: No such file or directory\n", redir->filename);
                return -1;
            }
            if (access(redir->filename, R_OK) == -1) {
                fprintf(stderr, "minishell: %s: Permission denied\n", redir->filename);
                return -1;
            }
            int tmp_fd = open(redir->filename, O_RDONLY);
            if (tmp_fd == -1) {
                perror(redir->filename);
                return -1;
            }
            if (fd != -1) close(fd);
            fd = tmp_fd;
        }
    }
    if (fd != -1) {
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    return 0;
}


int redirect_output(t_cmd *cmd)
{
    t_redir *redir = NULL;
    int fd = -1;
    // Use the last output redirection (as bash does)
    for (redir = cmd->outfiles; redir; redir = redir->next) {
        if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND) {
            if (access(redir->filename, F_OK) == 0 && access(redir->filename, W_OK) == -1) {
                fprintf(stderr, "minishell: %s: Permission denied\n", redir->filename);
                return -1;
            }
            char *dir = ft_strdup(redir->filename);
            char *last_slash = ft_strrchr(dir, '/');
            if (last_slash) {
                *last_slash = '\0';
                if (access(dir, W_OK) == -1) {
                    fprintf(stderr, "minishell: %s: Permission denied\n", dir);
                    free(dir);
                    return -1;
                }
            }
            free(dir);
            int flags = O_WRONLY | O_CREAT;
            if (redir->type == REDIR_APPEND)
                flags |= O_APPEND;
            else
                flags |= O_TRUNC;
            int tmp_fd = open(redir->filename, flags, 0644);
            if (tmp_fd == -1) {
                perror(redir->filename);
                return -1;
            }
            if (fd != -1) close(fd);
            fd = tmp_fd;
        }
    }
    if (fd != -1) {
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    return 0;
}