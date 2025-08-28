/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:26:36 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/28 12:36:21 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

void	minishell(char **env)
{
	t_shell	shell;

	ft_bzero(&shell, sizeof(t_shell));
	shell.env_list = init_env_list(env);
	run_shell_loop(&shell);
	free_env_list(&shell.env_list);
}
