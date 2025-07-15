#include "../include/minishell.h"

int handle_input_redirection(t_cmd *cmd)
{
    if (cmd->heredoc > 0) {
        if (dup2(cmd->heredoc, STDIN_FILENO) == -1) {
            perror("dup2 heredoc");
            return -1;
        }
        close(cmd->heredoc);
    }
    else if (cmd->infile) {
        int fd = open(cmd->infile, O_RDONLY);
        if (fd == -1) {
            perror(cmd->infile);
            return -1;
        }
        if (dup2(fd, STDIN_FILENO) == -1) {
            perror("dup2 input");
            close(fd);
            return -1;
        }
        close(fd);
    }
    return 0;
}

int handle_output_redirection(t_cmd *cmd)
{
    if (cmd->outfile) {
        int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
        int fd = open(cmd->outfile, flags, 0644);
        if (fd == -1) {
            perror(cmd->outfile);
            return -1;
        }
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2 output");
            close(fd);
            return -1;
        }
        close(fd);
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