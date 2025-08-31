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

void	init_main_shell(t_shell *shell, char **env)
{
	ft_bzero(shell, sizeof(t_shell));
	shell->env_list = init_env_list(env);
	shell->exit_status = 0;
	setup_signals(shell);
}

char	*get_input_and_handle_signals(t_shell *shell)
{
	char	*line;

	setup_signals(shell);
	line = readline("minishell> ");
	if (!line)
	{
		printf("exit\n");
		return (NULL);
	}
	return (line);
}

t_token	*handle_input_tokens(char *line, t_shell *shell)
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
	return (tokens);
}

static t_cmd	*parse_input_to_commands(char *line, t_shell *shell)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = handle_input_tokens(line, shell);
	if (!tokens)
		return (NULL);
	cmds = parse_tokens(tokens, shell);
	free_tokens(tokens);
	return (cmds);
}

void	handle_shell_input(char *line, t_shell *shell)
{
	t_cmd	*cmds;

	if (!line || !*line)
	{
		if (line)
			free(line);
		return ;
	}
	cmds = parse_input_to_commands(line, shell);
	if (!cmds)
	{
		free(line);
		return ;
	}
	setup_execution_signals();
	if (has_pipe(cmds))
		shell->exit_status = execute_pipeline(cmds, shell);
	else
		shell->exit_status = execute_command(cmds, shell);
	restore_shell_signals(shell);
	free_cmds(cmds);
	free(line);
}
