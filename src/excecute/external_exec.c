/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:15 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/25 20:20:36 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Forward declarations for static functions */
static void	try_shell_script(char *executable, t_cmd *cmd, char **env_array);
static void	cleanup_process_resources(char *executable, char **env_array);
static int	get_argc(char **argv);
static char	**create_sh_argv(char *executable, char **argv, int argc);

void	execute_process(char *executable, t_cmd *cmd, char **env_array)
{
    if (execve(executable, cmd->argv, env_array) == -1)
    {
        if (errno == ENOEXEC)
        {
            try_shell_script(executable, cmd, env_array);
        }
        else
        {
            perror("execve");
            cleanup_process_resources(executable, env_array);
            exit(127);
        }
    }
}

void	handle_exec_failure(char *executable, char **env_array)
{
    perror("execve");
    cleanup_process_resources(executable, env_array);
    exit(127);
}

static void	try_shell_script(char *executable, t_cmd *cmd, char **env_array)
{
    char	**new_argv;
    int		argc;

    argc = get_argc(cmd->argv);
    new_argv = create_sh_argv(executable, cmd->argv, argc);
    if (!new_argv)
    {
        cleanup_process_resources(executable, env_array);
        exit(127);
    }
    
    if (execve("/bin/sh", new_argv, env_array) == -1)
    {
        perror("execve /bin/sh");
        free_string_array(new_argv);
        cleanup_process_resources(executable, env_array);
        exit(127);
    }
}

static void	cleanup_process_resources(char *executable, char **env_array)
{
    if (executable && ft_strchr(executable, '/'))
        free(executable);
    
    if (env_array)
        free_string_array(env_array);
}

static int	get_argc(char **argv)
{
    int	count;

    count = 0;
    if (!argv)
        return (0);
    
    while (argv[count])
        count++;
    
    return (count);
}

static char	**create_sh_argv(char *executable, char **argv, int argc)
{
    char	**new_argv;
    int		i;

    new_argv = malloc(sizeof(char *) * (argc + 3));
    if (!new_argv)
        return (NULL);
    
    new_argv[0] = ft_strdup("sh");
    new_argv[1] = ft_strdup(executable);
    if (!new_argv[0] || !new_argv[1])
    {
        free_string_array(new_argv);
        return (NULL);
    }
    
    i = 1;
    while (i < argc)
    {
        new_argv[i + 1] = ft_strdup(argv[i]);
        if (!new_argv[i + 1])
        {
            free_string_array(new_argv);
            return (NULL);
        }
        i++;
    }
    new_argv[argc + 1] = NULL;
    return (new_argv);
}
