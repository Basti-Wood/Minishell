/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:05:38 by seftekha          #+#    #+#             */
/*   Updated: 2025/07/28 16:05:39 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

static void	init_main_shell(t_shell *shell, char **env)
{
	ft_bzero(shell, sizeof(t_shell));
	shell->env_list = init_env_list(env);
	shell->exit_status = 0;
}

static char	*get_input_and_handle_signals(t_shell *shell)
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
	{
		printf("exit\n");
		return (NULL);
	}
	if (g_signal_status == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		free(line);
		return ((char *)1);
	}
	return (line);
}

static t_token	*handle_input_tokens(char *line, t_shell *shell)
{
	t_token	*tokens;

	if (*line)
		add_history(line);
	shell->input = line;
	tokens = tokenize(shell->input, shell);
	if (!tokens)
		return (NULL);
	tokens = handle_empty_expansions(tokens);
	if (!tokens)
		return (NULL);
	tokens = expand_wildcards_in_tokens(tokens);
	return (tokens);
}

static void	handle_shell_input(char *line, t_shell *shell)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = handle_input_tokens(line, shell);
	if (!tokens)
	{
		free(line);
		return ;
	}
	cmds = parse_tokens(tokens, shell);
	free_tokens(tokens);
	if (cmds && has_pipe(cmds))
		shell->exit_status = execute_pipeline(cmds, shell);
	if (cmds && !has_pipe(cmds))
		shell->exit_status = execute_command(cmds, shell);
	if (cmds)
		free_cmds(cmds);
	free(line);
}

void	minishell(char **env)
{
	t_shell	shell;
	char	*line;

	init_main_shell(&shell, env);
	line = get_input_and_handle_signals(&shell);
	while (line)
	{
		if (line == (char *)1)
			continue ;
		handle_shell_input(line, &shell);
		line = get_input_and_handle_signals(&shell);
	}
	if (shell.env_list.head)
		free_env_list(&shell.env_list);
}
