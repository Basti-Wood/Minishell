/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seftekha <seftekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:04:21 by seftekha          #+#    #+#             */
/*   Updated: 2025/08/07 16:22:03 by seftekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *remove_quote_markers(const char *str)
{
	char *result;
	char *src;
	char *dst;

	if (!str || !(result = malloc(ft_strlen(str) + 1)))
		return (NULL);
	src = (char *)str;
	dst = result;
	while (*src)
	{
		if (*src > '\004' || *src < '\001')
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
	return (result);
}

static char *copy_string(char *dst, const char *src)
{
	while (*src)
		*dst++ = *src++;
	return (dst);
}

static char *get_var_name(char **src)
{
	char *start;
	char *result;
	int len;

	start = *src;
	len = 0;
	while (**src && (ft_isalnum(**src) || **src == '_'))
	{
		(*src)++;
		len++;
	}
	if (!(result = malloc(len + 1)))
		return (NULL);
	ft_memcpy(result, start, len);
	result[len] = '\0';
	return (result);
}

static char *expand_dollar(char **src, char *dst, t_shell *shell)
{
	char *var_name;
	char *var_value;
	char *temp;

	(*src)++;
	if (!**src)
		*dst++ = '$';
	else if (**src == '?')
	{
		temp = ft_itoa(shell->exit_status);
		dst = copy_string(dst, temp);
		free(temp);
		(*src)++;
	}
	else if (ft_isalpha(**src) || **src == '_')
	{
		var_name = get_var_name(src);
		var_value = get_env_value(shell->envp, var_name);
		if (var_value)
			dst = copy_string(dst, var_value);
		free(var_name);
	}
	else if (ft_isdigit(**src))
		(*src)++;
	else
		*dst++ = '$';
	return (dst);
}

char *expand_variables(char *str, t_shell *shell)
{
	char *result;
	char *src;
	char *dst;
	int in_single_quotes;

	if (!(result = malloc(ft_strlen(str) * 3 + 1)))
		return (NULL);
	src = str;
	dst = result;
	in_single_quotes = 0;
	while (*src)
	{
		if (*src == '\001')
			in_single_quotes = 1;
		else if (*src == '\003')
			in_single_quotes = 0;
		else if (*src == '$' && !in_single_quotes)
			dst = expand_dollar(&src, dst, shell);
		else if (*src != '\002' && *src != '\004')
			*dst++ = *src;
		if (*src)
			src++;
	}
	*dst = '\0';
	return (result);
}

t_token	*handle_empty_expansions(t_token *tokens)
{
    t_token	*cur;
    t_token	*prev;
    t_token	*tmp;

    cur = tokens;
    prev = NULL;

    while (cur)
    {
        if (cur->type == CMD && cur->str && cur->str[0] == '\0')
        {
            tmp = cur;
            cur = cur->next;
            if (cur)
            {
                cur->type = CMD;
                cur->prev = prev;
                if (prev)
                    prev->next = cur;
                else
                    tokens = cur;
            }
            else if (prev)
                prev->next = NULL;
            else
                tokens = NULL;
            free(tmp->str);
            free(tmp);
        }
        else
        {
            prev = cur;
            cur = cur->next;
        }
    }
    return (tokens);
}
