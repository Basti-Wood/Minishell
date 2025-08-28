/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:38:16 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/08 14:38:31 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	shift_argv(char ***argv)
{
	int	i;
	int	j;
	char **args;

	if (!argv || !*argv)
		return;

	args = *argv;
	i = 0;
	/* Skip empty strings at the start without freeing them here — ownership
	   of the strings belongs to the caller and freeing here can cause double
	   free / invalid pointer errors. */
	while (args[i] && args[i][0] == '\0')
		i++;

	if (!args[i])
	{
		args[0] = NULL;
		return;
	}

	j = 0;
	while (args[i])
	{
		args[j] = args[i];
		j++;
		i++;
	}
	args[j] = NULL;
}
