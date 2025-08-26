/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:02:28 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/26 10:45:35 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <errno.h>
# include <stdarg.h>
# include "../libft/libft/libft.h"

# define SINGLE_QUOTE_START '\001'
# define SINGLE_QUOTE_END '\002'
# define DOUBLE_QUOTE_START '\003'
# define DOUBLE_QUOTE_END '\004'

# define MAX_PATH_LEN 4096
# define INITIAL_CAPACITY 64

# define SUCCESS 0
# define FAILURE 1
# define MALLOC_ERROR 2
# define FORK_ERROR 3

typedef enum e_token_type
{
	EMPTY = 0,
	CMD = 1,
	ARG = 2,
	TRUNC = 3,
	APPEND = 4,
	INPUT = 5,
	HEREDOC = 6,
	PIPE = 7,
	END = 8
}	t_token_type;

typedef enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	char			*filename;
	t_redir_type	type;
	struct s_redir	*next;
}	t_redir;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_env_node
{
	char				*key;
	char				*value;
	struct s_env_node	*next;
}	t_env_node;

typedef struct s_env_list
{
	int			size;
	t_env_node	*head;
}	t_env_list;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*infiles;
	t_redir			*outfiles;
	t_redir			*redirs;
	int				heredoc;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_env_list	env_list;
	int			exit_status;
	char		*input;
}	t_shell;

typedef struct s_fork_data
{
	t_cmd	*cmds;
	int		**pipes;
	pid_t	*pids;
	int		cmd_count;
	t_shell	*shell;
}	t_fork_data;

typedef struct s_child_data
{
	int		i;
	int		cmd_count;
	int		**pipes;
	t_cmd	*current;
	t_shell	*shell;
}	t_child_data;

extern volatile sig_atomic_t	g_signal_status;

void		minishell(char **env);
void		handle_sigint(int sig);
void		setup_heredoc_signals(void);
void		setup_signals(t_shell *shell);
int			init_shell(t_shell *shell, char **env);
char		*get_input_line(void);
int			handle_interrupt_signal(char *line);
t_token		*tokenize(const char *input, t_shell *shell);
t_token		*token_split(char *input);
t_token		*create_token(char **elements, t_shell *shell);
t_token		*process_tokens(t_shell *shell);
void		type_arg(t_token *token);
char		**ft_split_quotes(const char *input);
int			handle_quoted_string(const char *s, int i, char quote,
				char **result);
int			skip_spaces(const char *s, int i);
int			count_tokens(const char *s);
char		*extract_token(const char *s, int *pos);
int			handle_quote_extraction(const char *s, int *i, char *result,
				int *res_len);
int			handle_operator(const char *s, int i, char **result, int *res_len);
int			handle_regular_char(const char *s, int i, char **result,
				int *res_len);
void		finalize_token_extraction(char *result, int res_len);
t_cmd		*parse_tokens(t_token *tokens, t_shell *shell);
t_cmd		*process_commands(t_token *tokens, t_shell *shell);
t_token		*handle_empty_expansions(t_token *tokens);
t_cmd		*create_cmd_node(void);
void		add_redirection(t_cmd *cmd, t_redir *redir);
t_redir		*create_redir(char *filename, t_redir_type type);
int			execute_command(t_cmd *cmd, t_shell *shell);
int			execute_builtin(t_cmd *cmd, t_shell *shell);
int			execute_external(t_cmd *cmd, t_shell *shell);
pid_t		fork_and_execute(t_cmd *cmd, t_shell *shell);
void		shift_argv(char ***argv);
int			execute_with_redirections(t_cmd *cmd, t_shell *shell);
int			execute_pipeline(t_cmd *cmds, t_shell *shell);
int			execute_cmds(t_cmd *cmds, t_shell *shell);
int			is_builtin(char *cmd);
char		*find_executable(char *cmd, t_env_list *env_list);
int			has_pipe(t_cmd *cmds);
void		setup_child_signals(void);
int			**create_pipes(int cmd_count);
void		free_pipes(int **pipes, int cmd_count);
char		*construct_path(char *dir, char *cmd);
int			builtin_echo(char **args);
int			builtin_cd(char **args, t_env_list *env_list);
int			builtin_pwd(void);
int			builtin_export(char **args, t_env_list *env_list);
int			builtin_unset(char **args, t_env_list *env_list);
int			builtin_env(t_env_list *env_list);
int			builtin_exit(char **args, t_shell *shell);
t_env_list	init_env_list(char **env);
char		*get_env_value(t_env_list *env_list, const char *key);
void		set_env_value(t_env_list *env_list, const char *key,
				const char *value);
char		**env_list_to_array(t_env_list *env_list);
t_env_node	*create_env_node(const char *key, const char *value);
void		remove_env_node(t_env_list *env_list, const char *key);
char		*expand_variables(char *str, t_shell *shell);
char		*remove_quote_markers(const char *str);
char		*get_variable_value(t_shell *shell, char *var_name);
int			should_expand_in_context(char c, int in_single_quotes);
int			handle_redirections_in_order(t_cmd *cmd);
int			handle_input_redir(t_token **tokens, t_cmd *cmd);
int			handle_heredoc(char *delimiter, t_shell *shell);
int			setup_heredoc(char *delimiter, int pipefd[2],
				char **final_delimiter);
void		heredoc_child_process(int pipefd, char *delimiter,
				int expand, t_shell *shell);
void		free_tokens(t_token *tokens);
void		free_redirs(t_redir *redirs);
void		free_argv(char **argv);
void		free_cmds(t_cmd *cmds);
void		free_env_list(t_env_list *env_list);
void		ft_error(char *message, int signal);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strndup(const char *s, size_t n);
char		*ft_strcpy(char *dest, const char *src);
char		*ft_strcat(char *dest, const char *src);
int			ft_isspace(int c);
int			ft_isnum(int c);
int			is_operator(char c);
int			is_quote(char c);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
int			ft_fprintf(int fd, const char *format, ...);
int			ft_fprintf_stderr(const char *format, ...);
void		ft_putstr_fd_len(char *s, int fd, int *len);
void		ft_putchar_fd_len(char c, int fd, int *len);
void		ft_putnbr_fd_len(int n, int fd, int *len);
void		handle_format_specifier(va_list args, char specifier,
				int fd, int *len);
int			ft_printf(const char *format, ...);
int			handle_output_redir(t_token **tokens, t_cmd *cmd);
int			handle_heredoc_redir(t_token **tokens, t_cmd *cmd, t_shell *shell);
void		add_to_ordered_redirs(t_cmd *cmd, t_redir *redir);
void		add_to_infiles(t_cmd *cmd, t_redir *redir);
void		add_to_outfiles(t_cmd *cmd, t_redir *redir);
int			add_argument(t_cmd *cmd, char *arg, int *argc);
int			is_redirection_token(t_token_type type);
int			handle_redirection(t_token **tokens, t_cmd *cmd, t_shell *shell);
t_cmd		*init_new_cmd(void);
int			setup_child_pipes(int **pipes, int cmd_count, int cmd_index);
void		close_parent_pipes(int **pipes, int cmd_count);
void		wait_for_children(pid_t *pids, int cmd_count, t_shell *shell);
void		handle_broken_pipe(int status, int is_last_cmd);
void		print_sorted_env(t_env_list *env_list);
int			execute_external_command(t_cmd *cmd, t_shell *shell);
void		free_paths_array(char **paths);
char		*build_full_path(char *dir, char *cmd);
char		*search_in_paths(char *cmd, char *path_env);
int			process_input_redir(redir, fd);

#endif
