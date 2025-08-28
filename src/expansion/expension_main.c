#include "../../include/minishell.h"

static char	*allocate_result_buffer(char *str)
{
	char	*result;
	int		size;

	size = ft_strlen(str) * 10 + 1024;
	result = malloc(size);
	if (!result)
		return (NULL);
	ft_memset(result, 0, size);
	return (result);
}

static int	handle_quote_marker(char c, int *in_single, int *in_double)
{
	if (c == '\001')
	{
		*in_single = 1;
		return (1);
	}
	else if (c == '\002')
	{
		*in_single = 0;
		return (1);
	}
	else if (c == '\003')
	{
		*in_double = 1;
		return (1);
	}
	else if (c == '\004')
	{
		*in_double = 0;
		return (1);
	}
	return (0);
}

void	handle_quote_markers(char c, int *in_single_quotes)
{
	if (c == '\001')
		*in_single_quotes = 1;
	else if (c == '\002')
		*in_single_quotes = 0;
}

char	*expand_variables(char *str, t_shell *shell)
{
	char	*result;
	char	*src;
	char	*dst;
	int		in_single;
	int		in_double;

	result = allocate_result_buffer(str);
	if (!result)
		return (NULL);
	src = str;
	dst = result;
	in_single = 0;
	in_double = 0;
	while (*src)
	{
		if (handle_quote_marker(*src, &in_single, &in_double))
		{
			*dst++ = *src++;
			continue;
		}
		if (*src == '$' && !in_single)
			dst = expand_dollar(&src, dst, shell);
		else
			*dst++ = *src++;
	}
	*dst = '\0';
	return (result);
}
