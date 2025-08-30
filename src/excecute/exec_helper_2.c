#include "../include/minishell.h"

int	get_argc(char **argv)
{
	int	count;

	count = 0;
	if (!argv)
		return (0);
	while (argv[count])
		count++;
	return (count);
}

int	save_file_descriptors(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
		return (-1);
	return (0);
}

void	restore_file_descriptors(int saved_stdin, int saved_stdout)
{
	if (saved_stdin >= 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout >= 0)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

int	execute_command(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	result;

	if (!cmd)
		return (0);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
		return (1);
	result = 0;
	if (handle_redirections(cmd) != 0)
		result = 1;
	else if (cmd->argv && cmd->argv[0])
	{
		if (is_builtin(cmd->argv[0]))
		{
			result = execute_builtin(cmd, shell);
		}
		else
			result = execute_external_command(cmd, shell);
	}
	restore_file_descriptors(saved_stdin, saved_stdout);
	return (result);
}

int	validate_all_redirects(t_cmd *cmd)
{
	t_redir	*current;

	if (!cmd || !cmd->redirs)
		return (0);
	current = cmd->redirs;
	while (current)
	{
		if (current->type == REDIR_INPUT && current->filename)
		{
			if (access(current->filename, F_OK) == -1)
			{
				ft_fprintf_stderr("minishell: %s: No such file or directory\n",
					current->filename);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}


