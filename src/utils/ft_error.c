#include "../include/minishell.h"

void ft_error(char *message, int signal)
{
	perror(message);
	exit(signal);
}