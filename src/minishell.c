#include "../include/minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

void	handle_sigint(int sig)
{
	g_signal_status = sig;
}

static void	init_shell(t_shell *shell, char **env)
{
	ft_bzero(shell, sizeof(t_shell));
	shell->envp = malloc(sizeof(t_env_list));
	if (!shell->envp)
	{
		perror("malloc");
		return ;
	}
	*shell->envp = init_env_list(env);
}

static char	*get_input_line(t_shell *shell)
{
	char	*line;

	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	if (g_signal_status == SIGINT)
	{
		shell->exit_status = 130;
		g_signal_status = 0;
	}
	line = readline("minishell> ");
	if (!line)
		printf("exit\n");
	return (line);
}

static int	handle_signal_interrupt(char *line)
{
	if (g_signal_status == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		free(line);
		return (1);
	}
	return (0);
}

static void	process_input(char *line, t_shell *shell)
{
	t_token	*tokens;
	t_cmd	*cmds;

	if (*line)
		add_history(line);
	shell->input = line;
	tokens = tokenize(shell);
	if (!tokens)
		return ;
	tokens = handle_empty_expansions(tokens);
	if (!tokens)
		return ;
	cmds = parse_tokens(tokens, shell);
	free_tokens(tokens);
	if (cmds)
	{
		if (has_pipe(cmds))
			shell->exit_status = execute_pipeline(cmds, shell);
		else
			shell->exit_status = execute_command(cmds, shell);
		free_cmds(cmds);
	}
}

void	minishell(char **env)
{
	t_shell	shell;
	char	*line;

	init_shell(&shell, env);
	if (!shell.envp)
		return ;
	while (1)
	{
		line = get_input_line(&shell);
		if (!line)
			break ;
		if (handle_signal_interrupt(line))
			continue ;
		process_input(line, &shell);
		free(line);
	}
	free_env_list(shell.envp);
}
