#include "../../include/minishell.h"

int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

int is_quote(char c)
{
    return (c == '\'' || c == '"');
}

// Skip spaces and return new position
int skip_spaces(const char *s, int i)
{
    while (s[i] && ft_isspace(s[i]))
        i++;
    return i;
}

// Count tokens with proper quote and concatenation handling
 int count_tokens(const char *s)
{
    int count = 0;
    int i = 0;
    int in_token = 0;
    char quote = 0;

    while (s[i])
    {
        i = skip_spaces(s, i);
        if (!s[i])
            break;

        // Check if we're starting a new token
        if (!in_token)
        {
            count++;
            in_token = 1;
        }

        if (is_quote(s[i]) && !quote)
        {
            quote = s[i++];
            while (s[i] && s[i] != quote)
                i++;
            if (s[i] == quote)
                i++;
            quote = 0;
        }
        else if (is_operator(s[i]))
        {
            if (!in_token || i == skip_spaces(s, 0))
            {
                count++;
            }
            if ((s[i] == '>' && s[i+1] == '>') || (s[i] == '<' && s[i+1] == '<'))
                i++;
            i++;
            in_token = 0;
        }
        else
        {
            while (s[i] && !ft_isspace(s[i]) && !is_operator(s[i]) && !is_quote(s[i]))
                i++;
        }

        // Check if the token continues (concatenation)
        if (s[i] && (is_quote(s[i]) || (!ft_isspace(s[i]) && !is_operator(s[i]))))
            continue;
        else
            in_token = 0;
    }
    return count;
}

// Extract a single token handling quotes and concatenation
// Store quote information by adding markers at beginning/end
char *extract_token(const char *s, int *pos)
{
    int i = *pos;
    char *result = malloc(2048);  // Larger buffer for quote markers
    int res_len = 0;
    char quote = 0;
    int token_started = 0;

    while (s[i])
    {
        if (is_quote(s[i]) && !quote)
        {
            quote = s[i++];
            token_started = 1;
            // Add quote marker to preserve quote type
            if (quote == '\'')
            {
                result[res_len++] = '\001';  // Start single quote marker
            }
            else
            {
                result[res_len++] = '\002';  // Start double quote marker
            }
            
            while (s[i] && s[i] != quote)
                result[res_len++] = s[i++];
            
            if (s[i] == quote)
            {
                i++;
                if (quote == '\'')
                    result[res_len++] = '\003';  // End single quote marker
                else
                    result[res_len++] = '\004';  // End double quote marker
            }
            quote = 0;
        }
        else if (is_operator(s[i]))
        {
            if (!token_started)
            {
                // Handle operator as separate token
                if ((s[i] == '>' && s[i+1] == '>') || (s[i] == '<' && s[i+1] == '<'))
                {
                    result[res_len++] = s[i++];
                    result[res_len++] = s[i++];
                }
                else
                {
                    result[res_len++] = s[i++];
                }
                break;
            }
            else
            {
                // End of current token when we hit an operator
                break;
            }
        }
        else if (ft_isspace(s[i]))
        {
            // End of token
            break;
        }
        else
        {
            // Regular character
            token_started = 1;
            result[res_len++] = s[i++];
        }

        // Check if we should continue concatenating
        if (!s[i] || (ft_isspace(s[i]) || is_operator(s[i])))
            break;
    }

    result[res_len] = '\0';
    *pos = i;

    char *final = ft_strdup(result);
    free(result);
    return final;
}
char **ft_split_quotes(const char *input)
{
    if (!input)
        return NULL;

    int count = count_tokens(input);
    char **tokens = malloc(sizeof(char *) * (count + 1));
    if (!tokens)
        return NULL;

    int i = 0;
    int token_idx = 0;

    while (input[i] && token_idx < count)
    {
        i = skip_spaces(input, i);
        if (!input[i])
            break;

        tokens[token_idx] = extract_token(input, &i);
        token_idx++;
    }

    tokens[token_idx] = NULL;
    return tokens;
}