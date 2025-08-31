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

#include "../../include/minishell.h"

void	shift_argv(char ***argv)
{
	int		i;
	int		j;
	char	**args;

	if (!argv || !*argv)
		return ;
	args = *argv;
	if (!args[0])
		return ;
	i = 0;
	j = 0;
	while (args[i])
	{
		if (args[i][0] != '\0')
		{
			args[j] = args[i];
			j++;
		}
		i++;
	}
	args[j] = NULL;
}
