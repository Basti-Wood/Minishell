#include "../include/minishell.h"

int handle_input_redirection(t_cmd *cmd)
{
    int fd = -1;
    t_redir *redir;
    // Handle all input redirections
    if (cmd->infiles) {
        for (redir = cmd->infiles; redir; redir = redir->next) {
            if (redir->type == REDIR_INPUT) {
                int tmp_fd = open(redir->filename, O_RDONLY);
                if (tmp_fd == -1) {
                    perror(redir->filename);
                    if (fd != -1) close(fd);
                    return -1;
                }
                if (fd != -1) close(fd);
                fd = tmp_fd;
            }
        }
        if (fd != -1) {
            if (dup2(fd, STDIN_FILENO) == -1) {
                perror("dup2 input");
                close(fd);
                return -1;
            }
            close(fd);
        }
    }
    // Handle heredoc as highest priority if present
    if (cmd->heredoc > 0) {
        if (dup2(cmd->heredoc, STDIN_FILENO) == -1) {
            perror("dup2 heredoc");
            return -1;
        }
        close(cmd->heredoc);
    }
    return 0;
}

int handle_output_redirection(t_cmd *cmd)
{
    int fd = -1;
    t_redir *redir;
    if (cmd->outfiles) {
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
                return -1;
            }
            if (fd != -1) close(fd);
            fd = tmp_fd;
        }
        if (fd != -1) {
            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("dup2 output");
                close(fd);
                return -1;
            }
            close(fd);
        }
    }
    return 0;
}

int validate_input_file(const char *filename)
{
    if (access(filename, F_OK) == -1) {
        fprintf(stderr, "minishell: %s: No such file or directory\n", filename);
        return 0;
    }
    if (access(filename, R_OK) == -1) {
        fprintf(stderr, "minishell: %s: Permission denied\n", filename);
        return 0;
    }
    return 1;
}

int validate_output_file(const char *filename)
{
    char *dir = ft_strdup(filename);
    char *last_slash = ft_strrchr(dir, '/');
    
    if (last_slash) {
        *last_slash = '\0';
        if (access(dir, W_OK) == -1) {
            fprintf(stderr, "minishell: %s: Permission denied\n", dir);
            free(dir);
            return 0;
        }
    }
    free(dir);
    return 1;
}