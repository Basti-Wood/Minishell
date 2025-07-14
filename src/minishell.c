#include "../include/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1); 
	rl_replace_line("", 0);   
	rl_on_new_line();     
	rl_redisplay();  
}

void minishell(char **env)
{
    t_shell shell;
    char *line;
    t_token *tokens;
    t_token *tmp;
    t_cmd *cmds;
    t_cmd *cmd_tmp;
    int i;

    // Initialize shell
    shell.exit_status = 0;
    shell.envp = malloc(sizeof(t_env_list));
    if (!shell.envp)
    {
        perror("malloc");
        return;
    }
    
    // Initialize environment from passed env parameter
    *shell.envp = init_env_list(env);

    // Set up signal handling
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);

    while (1)
    {
        line = readline("minishell> ");
        if (!line)
        {
            printf("exit\n");
            break;
        }

        if (*line)
            add_history(line);

        shell.input = line;
        tokens = tokenize(&shell);
        if (!tokens)
        {
            free(line);
            continue;
        }

        // Parse the tokens into commands
        cmds = parse_tokens(tokens, &shell);

        // Execute commands
        if (has_pipe(cmds))
        {
            shell.exit_status = execute_pipeline(cmds, &shell);
        }
        else
        {
            // Single command execution
            cmd_tmp = cmds;
            while (cmd_tmp)
            {
                shell.exit_status = execute_command(cmd_tmp, &shell);
                cmd_tmp = cmd_tmp->next;
            }
        }

        // Free commands
        while (cmds)
        {
            cmd_tmp = cmds;
            cmds = cmds->next;
            if (cmd_tmp->argv)
            {
                i = 0;
                while (cmd_tmp->argv[i])
                    free(cmd_tmp->argv[i++]);
                free(cmd_tmp->argv);
            }
            free(cmd_tmp->infile);
            free(cmd_tmp->outfile);
            if (cmd_tmp->heredoc > 0)
                close(cmd_tmp->heredoc);
            free(cmd_tmp);
        }

        // Free tokens
        while (tokens)
        {
            tmp = tokens;
            tokens = tokens->next;
            free(tmp->str);
            free(tmp);
        }

        free(line);
    }

    // Free environment
    t_env_node *env_tmp;
    while (shell.envp->head)
    {
        env_tmp = shell.envp->head;
        shell.envp->head = shell.envp->head->next;
        free(env_tmp->key);
        free(env_tmp->value);
        free(env_tmp);
    }
    free(shell.envp);
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

    // Check if delimiter is quoted
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

    if (pid == 0) {  // Child process
        struct sigaction sa;
        
        sa.sa_handler = heredoc_sigint_handler;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGINT, &sa, NULL);
        
        close(pipefd[0]);  // Close read end
        
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
    else {  // Parent process
        close(pipefd[1]);  // Close write end
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

int has_pipe(t_cmd *cmds)
{
    while (cmds)
    {
        if (cmds->next)
            return 1;
        cmds = cmds->next;
    }
    return 0;
}