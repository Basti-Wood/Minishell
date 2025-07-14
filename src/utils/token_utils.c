#include "../../include/minishell.h"

static int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

// First pass: Count how many tokens we will have. This avoids multiple reallocs.
static int count_tokens(const char *s)
{
    int count = 0;
    int i = 0;
    char quote;

    while (s[i])
    {
        while (ft_isspace(s[i]))
            i++;
        if (!s[i])
            break;
        count++;
        if (s[i] == '\'' || s[i] == '"')
        {
            quote = s[i++];
            while (s[i] && s[i] != quote)
                i++;
            if(s[i])
                i++;
        }
        else if (is_operator(s[i]))
        {
            if ((s[i] == '>' && s[i+1] == '>') || (s[i] == '<' && s[i+1] == '<'))
                i++;
            i++;
        }
        else
        {
            while (s[i] && !ft_isspace(s[i]) && !is_operator(s[i]) && s[i] != '\'' && s[i] != '"')
                i++;
        }
    }
    return (count);
}

// Second pass: Allocate memory and fill the tokens array.
static char **fill_tokens(char const *s, char **tokens, int count)
{
    int i = 0;
    int token_index = 0;
    char quote;
    int start;

    while (s[i] && token_index < count)
    {
        while (ft_isspace(s[i]))
            i++;
        if (!s[i])
            break;
        start = i;
        if (s[i] == '\'' || s[i] == '"')
        {
            quote = s[i++];
            while (s[i] && s[i] != quote)
                i++;
            if (s[i])
                i++;
        }
        else if (is_operator(s[i]))
        {
            if ((s[i] == '>' && s[i+1] == '>') || (s[i] == '<' && s[i+1] == '<'))
                i++;
            i++;
        }
        else
        {
            while (s[i] && !ft_isspace(s[i]) && !is_operator(s[i]) && s[i] != '\'' && s[i] != '"')
                i++;
        }
        tokens[token_index++] = ft_substr(s, start, i - start);
    }
    tokens[token_index] = NULL;
    return (tokens);
}


char **ft_split_quotes(const char *input)
{
    if (!input)
        return (NULL);

    int token_count = count_tokens(input);
    char **tokens = (char **)malloc(sizeof(char *) * (token_count + 1));
    if (!tokens)
        return (NULL);

    return (fill_tokens(input, tokens, token_count));
}