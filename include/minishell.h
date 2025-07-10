
#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/include/libft.h"
#include <stdio.h>
#include <stdlib.h>        
#include <unistd.h>        
#include <fcntl.h>  
#include <sys/types.h>       
#include <sys/stat.h>
#include <stdbool.h>       
#include <dirent.h>       
#include <sys/wait.h>     
#include <signal.h>     
#include <string.h>       
#include <termios.h>       
#include <sys/ioctl.h>     
#include <termcap.h>       
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include <limits.h>

#define EMPTY 0
#define CMD 1
#define ARG 2
#define TRUNC 3
#define APPEND 4
#define INPUT 5
#define PIPE 6
#define END 7

#define 	PARENT   1
#define 	CHILD   2
#define 	IGNORE   3
#define 	ERR_SYN_QUOTES   "Syntax error: quotes unclosed\n"
#define 	ERR_SYN_RD   "Syntax error: redirections\n"
#define 	ERR_SYN_PIPE   "Syntax error: pipes\n"
#define 	EXIT_SYN_QUOTES   2
#define 	EXIT_SYN_RD   2
#define 	EXIT_SYN_PIPE   2
#define 	OK   100
#define 	NOT_FOUND   101
#define 	FAILED   102
#define 	FAIL_SYSCALL   -1
#define 	OKAY   0
#define 	FAIL_SYSCALL_PARENT   1
#define 	FAIL_SYSCALL_CHILD   2

typedef struct s_token
{
	char *str;
	int type;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;

typedef struct s_cmd {
	char	**argv;
	int		in_fd;
	int		out_fd;
	char	*infile;
	char	*outfile;
	bool	append;
	struct s_cmd *next;
} t_cmd;

typedef struct s_env_list
{
    struct s_env_node       *head;
    int                         size;
}               t_env_list;
 
typedef struct s_env_node
{
    char                    *key;
    char                    *value;
    struct s_env_node   *next;
}               t_env_node;

typedef struct s_shell {
    char     *input;
    t_token  *tokens;
	t_cmd	*cmd;
    t_env_list	*envp;
    int       exit_status;
    pid_t     pid;
} t_shell;

void ft_error(char *message, int signal);
void minishell();
int	ft_strcmp(const char *s1, const char *s2);
void type_arg(t_token *token);
t_token *create_token(char **elements);
t_token *tokenize(t_shell *shell);
char	*ft_strndup(const char *s, size_t n);
char **ft_split_quotes(const char *input);
int token_count(const char *input);
t_cmd *parse_tokens(t_token *tokens);
t_env_list init_env_list(char **env);


#endif