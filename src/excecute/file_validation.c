#include "../../include/minishell.h"

int check_redirect_errors(t_redir *redirs)
{
	t_redir *current;

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_INPUT)
		{
			if (access(current->filename, F_OK) == -1)
			{
				ft_fprintf_stderr("minishell: %s: No such file or directory\n",
					current->filename);
				return (1);
			}
			if (access(current->filename, R_OK) == -1)
			{
				ft_fprintf_stderr("minishell: %s: Permission denied\n",
					current->filename);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

int validate_input_files_before_output(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (check_redirect_errors(cmd->redirs) != 0)
		return (1);
	return (0);
}

int validate_output_file(const char *filename)
{
	struct stat st;
	char *dir_path;
	char *last_slash;

	if (stat(filename, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			ft_fprintf_stderr("minishell: %s: Is a directory\n", filename);
			return (1);
		}
	}
	dir_path = ft_strdup(filename);
	if (!dir_path)
		return (1);
	last_slash = ft_strrchr(dir_path, '/');
	if (last_slash && last_slash != dir_path)
	{
		*last_slash = '\0';
		if (access(dir_path, F_OK) == -1)
		{
			ft_fprintf_stderr("minishell: %s: No such file or directory\n",
				filename);
			free(dir_path);
			return (1);
		}
	}
	free(dir_path);
	return (0);
}