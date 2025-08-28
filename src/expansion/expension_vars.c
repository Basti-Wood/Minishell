#include "../../include/minishell.h"

char *ft_strncpy(char *dest, const char *src, size_t n) {
    char *d = dest;
    const char *s = src;

    // Copy characters until n runs out or we hit '\0'
    while (n > 0 && *s != '\0') {
        *d++ = *s++;
        n--;
    }

    // Pad with '\0' if needed
    while (n > 0) {
        *d++ = '\0';
        n--;
    }

    return dest;
}

char	*get_var_name(char **src)
{
	char	*start;
	int		len;
	char	*var_name;

	start = *src;
	len = 0;
	while (**src && (ft_isalnum(**src) || **src == '_'))
	{
		(*src)++;
		len++;
	}
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, start, len);
	var_name[len] = '\0';
	return (var_name);
}

char	*copy_string_to_dst(char *dst, const char *src)
{
	if (!dst || !src)
		return (dst);
	while (*src)
		*dst++ = *src++;
	return (dst);
}
