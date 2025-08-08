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

static void	shift_argv(char ***argv)
{
	int	i;
	int	j;

	i = 0;
	while ((*argv)[i] && (*argv)[i][0] == '\0')
	{
		free((*argv)[i]);
		i++;
	}
	if (!(*argv)[i])
	{
		free((*argv)[0]);
		(*argv)[0] = NULL;
		return ;
	}
	j = 0;
	while ((*argv)[i])
	{
		(*argv)[j] = (*argv)[i];
		j++;
		i++;
	}
	(*argv)[j] = NULL;
}
