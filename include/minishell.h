/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:02:28 by seftekha          #+#    #+#             */
/*   Updated: 2025/09/01 16:39:55 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/ft_printf.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <glob.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

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
}						t_token_type;

typedef enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redir_type;

typedef struct s_redir
{
	char				*filename;
	t_redir_type		type;
	struct s_redir		*next;
}						t_redir;

typedef struct s_token
{
	char				*str;
	t_token_type		type;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef struct s_env_node
{
	char				*key;
	char				*value;
	struct s_env_node	*next;
}						t_env_node;

typedef struct s_env_list
{
	int					size;
	t_env_node			*head;
}						t_env_list;

typedef struct s_cmd
{
	char				**argv;
	t_redir				*infiles;
	t_redir				*outfiles;
	t_redir				*redirs;
	int					heredoc;
	struct s_cmd		*next;
}						t_cmd;

char					*path_search(const char *cmd, char **envp);

typedef struct s_shell
{
	t_env_list			env_list;
	int					exit_status;
	int					should_exit;
	char				*input;
}						t_shell;

typedef struct s_heredoc_data
{
	int					pipe_fd;
	char				*delimiter;
	int					expand;
	t_shell				*shell;
}						t_heredoc_data;

typedef struct s_fork_data
{
	t_cmd				*cmds;
	int					**pipes;
	pid_t				*pids;
	int					cmd_count;
	t_shell				*shell;
}						t_fork_data;

typedef struct s_child_data
{
	int					i;
	int					cmd_count;
	int					**pipes;
	t_cmd				*current;
	t_shell				*shell;
}						t_child_data;

int						minishell(char **env);
void					init_main_shell(t_shell *shell, char **env);
char					*get_input_and_handle_signals(t_shell *shell);
void					cleanup_static_vars(void);
void					handle_shell_input(char *line, t_shell *shell);
void					handle_sigint(int sig);
void					setup_heredoc_signals(void);
void					setup_signals(t_shell *shell);
int						init_shell(t_shell *shell, char **env);
char					*get_input_line(void);
int						handle_interrupt_signal(char *line);
t_token					*tokenize(const char *input, t_shell *shell);
t_token					*token_split(char *input);
t_token					*create_token(char **elements, t_shell *shell);
t_token					*process_tokens(t_shell *shell);
void					type_arg(t_token *token);
char					**ft_split_quotes(const char *input);
int						handle_quoted_string(const char *s, int i, char quote,
							char **result);
int						skip_spaces(const char *s, int i);
int						count_tokens(const char *s);
char					*extract_token(const char *s, int *pos);
int						handle_quote_extraction(const char *s, int *i,
							char *result, int *res_len);
int						handle_operator(const char *s, int i, char **result,
							int *res_len);
int						handle_regular_char(const char *s, int i, char **result,
							int *res_len);
void					finalize_token_extraction(char *result, int res_len);
void					free_token(t_token *token);
void					free_token_list(t_token *head);
t_token					*create_new_token(char *str, t_token_type type);
void					append_token(t_token **head, t_token *new_token);
t_cmd					*parse_tokens(t_token *tokens, t_shell *shell);
t_cmd					*parse_command_line(t_token *tokens, t_shell *shell);
t_cmd					*parse_pipeline(t_token **tokens, t_shell *shell);
t_cmd					*process_commands(t_token *tokens, t_shell *shell);
t_token					*handle_empty_expansions(t_token *tokens);
t_cmd					*create_cmd_node(void);
void					add_redirection(t_cmd *cmd, t_redir *redir);
t_redir					*create_redir(char *filename, t_redir_type type);
int						execute_command(t_cmd *cmd, t_shell *shell);
int						execute_builtin(t_cmd *cmd, t_shell *shell);
int						execute_external(t_cmd *cmd, t_shell *shell);
pid_t					fork_and_execute(t_cmd *cmd, t_shell *shell);
void					shift_argv(char ***argv);
int						execute_with_redirections(t_cmd *cmd, t_shell *shell);
int						execute_pipeline(t_cmd *cmds, t_shell *shell);
int						execute_cmds(t_cmd *cmds, t_shell *shell);
int						pipe_create(int pipe_fds[2]);
int						pipe_execute_command(t_cmd *cmd, int *pipe_in,
							int *pipe_out, t_shell *shell);
int						pipe_wait_for_children(pid_t *pids, int count,
							t_shell *shell);
int						is_builtin(char *cmd);
char					*find_executable(char *cmd, t_env_list *env_list);
int						has_pipe(t_cmd *cmds);
void					setup_child_signals(void);
int						**create_pipes(int cmd_count);
void					free_pipes(int **pipes, int cmd_count);
char					*construct_path(char *dir, char *cmd);
int						builtin_echo(char **args);
int						builtin_cd(char **args, t_env_list *env_list);
int						builtin_pwd(void);
int						builtin_export(char **args, t_env_list *env_list);
int						builtin_unset(char **args, t_env_list *env_list);
int						builtin_env(t_env_list *env_list);
int						builtin_exit(char **args, t_shell *shell);
t_env_list				init_env_list(char **env);
char					*get_env_value(t_env_list *env_list, const char *key);
void					set_env_value(t_env_list *env_list, const char *key,
							const char *value);
char					**env_list_to_array(t_env_list *env_list);
int						create_env_array(t_shell *shell, char ***env_array);
t_env_node				*create_env_node(const char *key, const char *value);
void					remove_env_node(t_env_list *env_list, const char *key);
char					*expand_variables(char *str, t_shell *shell);
char					*expand_dollar(char **src, char *dst, t_shell *shell);
char					*remove_quote_markers(const char *str);
char					*get_variable_value(t_shell *shell, char *var_name);
int						should_expand_in_context(char c, int in_single_quotes);
int						validate_input_file(const char *filename);
int						open_input_file(const char *filename);
int						handle_input_redirection(t_redir *redir);
int						apply_input_redirections(t_cmd *cmd);
int						restore_input(int saved_fd);
int						validate_output_file(const char *filename);
int						open_output_file(const char *filename, int flags);
int						handle_output_redirection(t_redir *redir);
int						apply_output_redirections(t_cmd *cmd);
int						save_io_fds(int *saved_stdin, int *saved_stdout);
void					restore_io_fds(int saved_stdin, int saved_stdout);
int						handle_all_redirections(t_cmd *cmd);
int						handle_redirections(t_cmd *cmd);
int						setup_pipe_redirections(t_cmd *cmd, int *pipe_fds);
void					cleanup_pipe_redirections(int *pipe_fds);
int						handle_input_redir(t_token **tokens, t_cmd *cmd);
int						handle_output_redir(t_token **tokens, t_cmd *cmd);
int						handle_heredoc(char *delimiter, t_shell *shell);
int						setup_heredoc(char *delimiter, int pipefd[2],
							char **final_delimiter);
void					heredoc_child_process(int pipefd, char *delimiter,
							int expand, t_shell *shell);
void					free_tokens(t_token *tokens);
void					free_redirs(t_redir *redirs);
void					free_argv(char **argv);
void					free_cmds(t_cmd *cmds);
void					free_env_list(t_env_list *env_list);
void					ft_error(char *message, int signal);
int						ft_strcmp(const char *s1, const char *s2);
char					*ft_strndup(const char *s, size_t n);
char					*ft_strcpy(char *dest, const char *src);
char					*ft_strcat(char *dest, const char *src);
char					*ft_strncpy(char *dest, const char *src, size_t n);
int						ft_isspace(int c);
int						ft_isnum(int c);
int						is_operator(char c);
int						is_quote(char c);
size_t					ft_strlcat(char *dst, const char *src, size_t dstsize);
int						ft_fprintf(int fd, const char *format, ...);
int						ft_fprintf_stderr(const char *format, ...);
void					ft_putstr_fd_len(char *s, int fd, int *len);
void					ft_putchar_fd_len(char c, int fd, int *len);
void					ft_putnbr_fd_len(int n, int fd, int *len);
void					handle_format_specifier(va_list args, char specifier,
							int fd, int *len);
int						ft_printf(const char *format, ...);
int						handle_heredoc_redir(t_token **tokens, t_cmd *cmd,
							t_shell *shell);
void					add_to_ordered_redirs(t_cmd *cmd, t_redir *redir);
void					add_to_infiles(t_cmd *cmd, t_redir *redir);
void					add_to_outfiles(t_cmd *cmd, t_redir *redir);
int						add_argument(t_cmd *cmd, char *arg, int *argc);
int						is_redirection_token(t_token_type type);
int						handle_redirection(t_token **tokens, t_cmd *cmd,
							t_shell *shell);
t_cmd					*init_new_cmd(void);
int						setup_child_pipes(int i, int cmd_count, int **pipes);
void					execute_child_external(t_cmd *current, t_shell *shell);
void					close_parent_pipes(int **pipes, int cmd_count);
void					wait_for_children(pid_t *pids, int cmd_count,
							t_shell *shell);
void					close_all_pipes(int **pipes, int cmd_count);
void					close_all_child_pipes(int **pipes, int cmd_count);
void					cleanup_pipes(int **pipes, int cmd_count);
void					setup_child_process(t_child_data *data);
void					execute_child_command(t_cmd *current, t_shell *shell);
int						execute_single_child(t_fork_data *data, int i,
							t_cmd *current);
int						allocate_resources(int ***pipes, pid_t **pids,
							int cmd_count);
void					cleanup_resources(int **pipes, pid_t *pids,
							int cmd_count);
int						count_commands(t_cmd *cmds);
int						**allocate_pipes(int cmd_count);
void					handle_broken_pipe(int status, int is_last_cmd);
void					print_sorted_env(t_env_list *env_list);
int						execute_external_command(t_cmd *cmd, t_shell *shell);
void					free_paths_array(char **paths);
char					*build_full_path(char *dir, char *cmd);
char					*search_in_paths(char *cmd, char *path_env);
int						process_input_redir(t_redir *redir, int *fd);
t_cmd					*process_command_tokens(t_token **tokens,
							t_shell *shell);
void					try_shell_script(char *executable, t_cmd *cmd,
							char **env_array);
void					cleanup_process_resources(char *executable,
							char **env_array);
void					handle_exec_failure(char *executable, char **env_array);
int						get_argc(char **argv);
char					**create_sh_argv(char *executable, char **argv,
							int argc);
void					free_string_array(char **array);
int						validate_redir_list(t_redir *redirs);
int						apply_input_fd(int fd);
int						process_output_redir(t_redir *redir, int *fd);
char					*get_var_name(char **src);
char					**expand_wildcard(char *pattern);
int						has_wildcard(char *str);
t_token					*expand_wildcards_in_tokens(t_token *tokens);
t_token					*replace_with_expanded(t_token *current,
							char **expanded);
char					*copy_string_to_dst(char *dst, const char *src);
int						handle_operator_extraction(const char *s, int *i,
							char *result, int *res_len);
int						export_single_var(char *arg, t_env_list *env_list);
void					safe_free(void **ptr);
char					*safe_strdup(const char *s);
void					*safe_malloc(size_t size);
int						safe_close(int fd);
void					safe_dup2(int oldfd, int newfd);
int						is_valid_identifier(const char *str);
void					print_export_env(t_env_list *env_list);
void					add_quote_markers(char quote, char *result,
							int *res_len, int is_end);
t_token					*assign_token_types(t_token *tokens);
int						handle_operator_in_count(const char *s, int *i,
							int *count, int *in_token);
int						handle_quote_in_count(const char *s, int *i,
							char *quote);
int						validate_single_redir(t_redir *redir);
int						validate_output_redir(t_redir *redir);
int						validate_input_redir(t_redir *redir);
int						save_file_descriptors(int *saved_stdin,
							int *saved_stdout);
void					restore_file_descriptors(int saved_stdin,
							int saved_stdout);
int						apply_output_redirection(t_redir *redir, int flags);
void					handle_quote_markers(char c, int *in_single_quotes);
int						should_copy_char(char c);
int						validate_input_files_before_output(t_cmd *cmd);
int						apply_redirections(t_cmd *cmd);
int						handle_single_redirection(t_redir *redir);
void					execute_process(char *executable, t_cmd *cmd,
							char **env_array);
void					setup_heredoc_signals_internal(void);
pid_t					fork_heredoc_process(int pipefd[2], char *tmp_delim);
int						setup_heredoc_pipe(int pipefd[2], char *tmp_delim);
int						prepare_heredoc_delimiter(char *delimiter,
							char **tmp_delim, int *expand);
void					heredoc_sigint_handler(int sig);
void					heredoc_child_main(t_heredoc_data *data);
int						heredoc_parent_wait(pid_t pid, int pipefd[2],
							char *tmp_delim);
int						process_heredoc_line(char *line, t_heredoc_data *data);
char					*read_heredoc_line(void);
int						process_heredoc_content(char *delimiter, t_shell *shell,
							int pipe_fd, int expand);
int						validate_single_redir(t_redir *redir);
int						validate_output_redir(t_redir *redir);
int						validate_input_redir(t_redir *redir);
void					add_redirection(t_cmd *cmd, t_redir *redir);
void					add_to_infile_list(t_cmd *cmd, t_redir *redir);
void					add_to_outfile_list(t_cmd *cmd, t_redir *redir);
void					link_tokens(t_token **head, t_token **current,
							t_token *new_tokens);
void					append_token(t_token **head, t_token *new_token);
t_token					*create_new_token(char *str, t_token_type type);
void					free_token_list(t_token *head);
void					free_token(t_token *token);
int						has_quote_markers(char *str);
int						apply_input_redir_safe(t_redir *redir,
							int *saved_stdin);
int						apply_output_redir_safe(t_redir *redir,
							int *saved_stdout);
int						apply_input_redir(t_redir *redir);
int						apply_output_redir(t_redir *redir);
int						apply_append_redir(t_redir *redir);
t_token					*create_expanded_token(char *str, t_token_type type);
void					insert_token_after(t_token *current,
							t_token *new_token);

void					setup_execution_signals(void);
void					restore_shell_signals(t_shell *shell);
int						execute_pipeline(t_cmd *cmds, t_shell *shell);
int						count_commands(t_cmd *cmds);

extern volatile sig_atomic_t	g_signal_status;

#endif
