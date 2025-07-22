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

// NEW FUNCTION: Validate input files before creating any output files
int validate_input_files_before_output(t_cmd *cmd)
{
    t_redir *redir;
    
    // Check all input files in the ordered redirs list (from parser)
    if (cmd->redirs) {
        for (redir = cmd->redirs; redir; redir = redir->next) {
            if (redir->type == REDIR_INPUT) {
                if (access(redir->filename, F_OK) == -1) {
                    ft_fprintf_stderr("minishell: %s: No such file or directory\n", redir->filename);
                    return -1;
                }
                if (access(redir->filename, R_OK) == -1) {
                    ft_fprintf_stderr("minishell: %s: Permission denied\n", redir->filename);
                    return -1;
                }
            }
        }
    }
    
    // Also check the infiles list for backward compatibility
    if (cmd->infiles) {
        for (redir = cmd->infiles; redir; redir = redir->next) {
            if (redir->type == REDIR_INPUT) {
                if (access(redir->filename, F_OK) == -1) {
                    ft_fprintf_stderr("minishell: %s: No such file or directory\n", redir->filename);
                    return -1;
                }
                if (access(redir->filename, R_OK) == -1) {
                    ft_fprintf_stderr("minishell: %s: Permission denied\n", redir->filename);
                    return -1;
                }
            }
        }
    }
    return 0;
}

int validate_input_file(const char *filename)
{
    if (access(filename, F_OK) == -1) {
        ft_fprintf_stderr("minishell: %s: No such file or directory\n", filename);
        return 0;
    }
    if (access(filename, R_OK) == -1) {
        ft_fprintf_stderr("minishell: %s: Permission denied\n", filename);
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
            ft_fprintf_stderr("minishell: %s: Permission denied\n", dir);
            free(dir);
            return 0;
        }
    }
    free(dir);
    return 1;
}

// MISSING FUNCTION: Remove quote markers from tokenized strings
char *remove_quote_markers(const char *str)
{
    if (!str)
        return NULL;
        
    int len = ft_strlen(str);
    char *result = malloc(len + 1);
    if (!result)
        return NULL;
        
    int i = 0, j = 0;
    
    while (str[i])
    {
        // Skip quote markers
        if (str[i] == '\001' || str[i] == '\002' || str[i] == '\003' || str[i] == '\004')
        {
            i++;
            continue;
        }
        
        result[j++] = str[i++];
    }
    
    result[j] = '\0';
    return result;
}