#include "../include/minishell.h"

volatile sig_atomic_t g_signal_status = 0;

void	handle_sigint(int sig)
{
	g_signal_status = sig;
}

void minishell(char **env)
{
    t_shell shell;
    char *line;
    t_token *tokens;
    t_cmd *cmds;

    ft_bzero(&shell, sizeof(t_shell));
    shell.envp = malloc(sizeof(t_env_list));
    if (!shell.envp)
    {
        perror("malloc");
        return;
    }
    *shell.envp = init_env_list(env);

    while (1)
    {
        signal(SIGINT, handle_sigint);
        signal(SIGQUIT, SIG_IGN);

        if (g_signal_status == SIGINT)
        {
            shell.exit_status = 130;
            g_signal_status = 0;
        }
        
        line = readline("minishell> ");
        if (!line)
        {
            printf("exit\n");
            break;
        }    

        if (g_signal_status == SIGINT)
        {
            rl_replace_line("", 0);
            rl_on_new_line();
            rl_redisplay();
            free(line);
            continue;
        }

        if (*line)
            add_history(line);

        shell.input = line;
		tokens = tokenize(&shell);
		if (!tokens)
			continue;
			
		// Handle empty expansions
		tokens = handle_empty_expansions(tokens);
		if (!tokens)
			continue;

		// Parse tokens into commands

		cmds = parse_tokens(tokens, &shell);
		free_tokens(tokens);

        if (cmds)
        {
            if (has_pipe(cmds))
                shell.exit_status = execute_pipeline(cmds, &shell);
            else
                shell.exit_status = execute_command(cmds, &shell);
			free_cmds(cmds);
        }
        free(line);
    }
	free_env_list(shell.envp);
}

static void heredoc_sigint_handler(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    exit(130);
}

int handle_heredoc(char *delimiter, t_shell *shell)
{
    int pipefd[2];
    char *line;
    pid_t pid;
    int status;
    int expand = 1;
    char *tmp_delim = NULL;

    if (delimiter[0] == '\'' || delimiter[0] == '"') {
        expand = 0;
        tmp_delim = ft_strtrim(delimiter, "\"'");
        if (!tmp_delim) {
            perror("malloc");
            return -1;
        }
        delimiter = tmp_delim;
    }

    if (pipe(pipefd) == -1) {
        perror("pipe");
        if (tmp_delim) free(tmp_delim);
        return -1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        if (tmp_delim) free(tmp_delim);
        return -1;
    }

    if (pid == 0)
	{
        struct sigaction sa;
        
        sa.sa_handler = heredoc_sigint_handler;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGINT, &sa, NULL);
        
        close(pipefd[0]);
        
        while (1) {
            line = readline("> ");
            if (!line) {
                ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n", STDERR_FILENO);
                break;
            }
            
            if (ft_strcmp(line, delimiter) == 0) {
                free(line);
                break;
            }
            
            if (expand) {
                char *expanded = expand_variables(line, shell);
                free(line);
                if (!expanded) {
                    close(pipefd[1]);
                    exit(1);
                }
                write(pipefd[1], expanded, ft_strlen(expanded));
                write(pipefd[1], "\n", 1);
                free(expanded);
            } else {
                write(pipefd[1], line, ft_strlen(line));
                write(pipefd[1], "\n", 1);
                free(line);
            }
        }
        close(pipefd[1]);
        exit(0);
    }
    else {
        close(pipefd[1]);
        waitpid(pid, &status, 0);
        
        if (tmp_delim) free(tmp_delim);
        
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 130) {
                close(pipefd[0]);
                return -1;
            }
            return pipefd[0];
        }
        close(pipefd[0]);
        return -1;
    }
}

