#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <curses.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <errno.h>
#include "../libft/libft/libft.h"

typedef enum e_token_type {
    EMPTY = 0,
    CMD = 1,
    ARG = 2,
    TRUNC = 3,
    APPEND = 4,
    INPUT = 5,
    HEREDOC = 6,
    PIPE = 7,
    END = 8
} t_token_type;

typedef enum e_redir_type {
    REDIR_INPUT,
    REDIR_OUTPUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} t_redir_type;

typedef struct s_token {
    char *str;
    t_token_type type;
    struct s_token *next;
    struct s_token *prev;
} t_token;

typedef struct s_env_node {
    char *key;
    char *value;
    struct s_env_node *next;
} t_env_node;

typedef struct s_env_list {
    t_env_node *head;
    int size;
} t_env_list;


typedef struct s_cmd {
    char **argv;
    char *infile;
    char *outfile;
    int heredoc;
    bool append;
    struct s_cmd *next;
} t_cmd;

typedef struct s_shell {
    char *input;
    t_env_list *envp;
    int exit_status;
} t_shell;

void minishell(char **env);
void handle_sigint(int sig);
int handle_heredoc(char *delimiter, t_shell *shell);
int has_pipe(t_cmd *cmds);

t_token *tokenize(t_shell *shell);
t_token *token_split(char *input);
t_token *create_token(char **elements, t_shell *shell);
void type_arg(t_token *token);
char **ft_split_quotes(const char *input);
int handle_quoted_string(const char *s, int i, char quote, char **result);

t_cmd *parse_tokens(t_token *tokens, t_shell *shell);

t_env_list init_env_list(char **env);
char *get_env_value(t_env_list *env_list, const char *key);
void set_env_value(t_env_list *env_list, const char *key, const char *value);
char **env_list_to_array(t_env_list *env_list);

int builtin_echo(char **args);
int builtin_cd(char **args, t_env_list *env_list);
int builtin_pwd(void);
int builtin_export(char **args, t_env_list *env_list);
int builtin_unset(char **args, t_env_list *env_list);
int builtin_env(t_env_list *env_list);
int builtin_exit(char **args);

int execute_command(t_cmd *cmd, t_shell *shell);
int execute_builtin(t_cmd *cmd, t_shell *shell);
int execute_external(t_cmd *cmd, t_shell *shell);
int execute_pipeline(t_cmd *cmds, t_shell *shell);
int is_builtin(char *cmd);
char *find_executable(char *cmd, t_env_list *env_list);

char *expand_variables(char *str, t_shell *shell);

void ft_error(char *message, int signal);
int ft_strcmp(const char *s1, const char *s2);
char *ft_strndup(const char *s, size_t n);
char *ft_strcpy(char *dest, const char *src);
char *ft_strcat(char *dest, const char *src);
int ft_isspace(int c);
int	ft_isnum(int c);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
char *remove_quote_markers(const char *str);

int handle_input_redirection(t_cmd *cmd);
int handle_output_redirection(t_cmd *cmd);
int validate_input_file(const char *filename);
int validate_output_file(const char *filename);

void free_tokens(t_token *tokens);
void free_cmds(t_cmd *cmds);
void free_env_list(t_env_list *env_list);



#endif