#include "../../include/minishell.h"

int token_count(const char *input)
{
    int i = 0, count = 0;
    char quote;

    while (input[i])
    {
        while (input[i] == ' ')
            i++;
        if (!input[i])
            break;
        count++;
        if (input[i] == '\'' || input[i] == '"')
        {
            quote = input[i++];
            while (input[i] && input[i] != quote)
                i++;
            if (input[i] == quote)
                i++;
        }
        else
        {
            while (input[i] && input[i] != ' ' && input[i] != '\'' && input[i] != '"')
                i++;
        }
    }
    return count;
}
char **ft_split_quotes(const char *input)
{
    int count = token_count(input);
    char **result = malloc(sizeof(char *) * (count + 1));
    if (!result)
    {
        perror("malloc");
    }

    int i = 0, j = 0;
    int start;
    char quote;

    while (input[i])
    {
        while (input[i] == ' ')
            i++;
        if (!input[i])
            break;

        if (input[i] == '\'' || input[i] == '"')
        {
            quote = input[i++];
            start = i;
            while (input[i] && input[i] != quote)
                i++;
            if (!input[i])
            {
                perror("Unclosed quote");
            }

            result[j] = ft_strndup(&input[start], i - start);
            if (!result[j])
            {
                perror("ft_strndup");
            }
            j++;
            i++;
        }
        else
        {
            start = i;
            while (input[i] && input[i] != ' ' && input[i] != '\'' && input[i] != '"')
                i++;

            result[j] = ft_strndup(&input[start], i - start);
            if (!result[j])
            {
                perror("ft_strndup");
            }
            j++;
        }
    }

    result[j] = NULL;
    return result;
}
