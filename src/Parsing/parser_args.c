/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:49:41 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:28 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**realloc_argv(char **argv, int old_size, int new_size)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (new_size + 1));
	if (!new_argv)
		return (NULL);
	i = 0;
	while (i < old_size && argv[i])
	{
		new_argv[i] = argv[i];
		i++;
	}
	while (i <= new_size)
		new_argv[i++] = NULL;
	if (argv)
		free(argv);
	return (new_argv);
}

int	add_argument(t_cmd *cmd, char *arg, int *argc)
{
	char	**new_argv;
	int		new_size;

	if (!cmd || !arg || !argc)
		return (0);
	new_size = *argc + 1;
	new_argv = realloc_argv(cmd->argv, *argc, new_size);
	if (!new_argv)
		return (0);
	new_argv[*argc] = ft_strdup(arg);
	if (!new_argv[*argc])
	{
		free(new_argv);
		return (0);
	}
	cmd->argv = new_argv;
	(*argc)++;
	return (1);
}
