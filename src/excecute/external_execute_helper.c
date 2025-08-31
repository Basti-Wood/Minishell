/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_execute_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:17:16 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/27 15:25:34 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_exec_failure(char *executable, char **env_array)
{
	perror("execve");
	cleanup_process_resources(executable, env_array);
	exit(127);
}
