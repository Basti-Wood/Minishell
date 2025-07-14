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
#include "../../include/minishell.h"

char **ft_split_quotes(const char *input) {
    if (!input) return NULL;

    char **tokens = NULL;
    int count = 0;
    char *current_token = NULL;
    size_t token_len = 0;
    size_t token_cap = 0;
    int in_quote = 0;
    char quote_char = 0;

    for (const char *p = input; *p; p++) {
        if (in_quote) {
            if (*p == quote_char) {
                in_quote = 0;
            } else {
                // Append to current_token
                if (token_len + 1 >= token_cap) {
                    token_cap = token_cap ? token_cap * 2 : 32;
                    char *new_token = realloc(current_token, token_cap);
                    if (!new_token) {
                        perror("malloc");
                        if (current_token) free(current_token);
                        for (int i = 0; i < count; i++) free(tokens[i]);
                        free(tokens);
                        return NULL;
                    }
                    current_token = new_token;
                }
                current_token[token_len++] = *p;
            }
        } else {
            if (ft_isspace(*p)) {
                if (current_token) {
                    // Finish current token
                    current_token[token_len] = '\0';
                    char **new_tokens = realloc(tokens, sizeof(char*) * (count + 1));
                    if (!new_tokens) {
                        perror("realloc");
                        free(current_token);
                        for (int i = 0; i < count; i++) free(tokens[i]);
                        free(tokens);
                        return NULL;
                    }
                    tokens = new_tokens;
                    tokens[count++] = current_token;
                    current_token = NULL;
                    token_len = 0;
                    token_cap = 0;
                }
                // Skip space
            } else if (*p == '\'' || *p == '"') {
                in_quote = 1;
                quote_char = *p;
                if (!current_token) {
                    current_token = malloc(32);
                    if (!current_token) {
                        perror("malloc");
                        for (int i = 0; i < count; i++) free(tokens[i]);
                        free(tokens);
                        return NULL;
                    }
                    token_cap = 32;
                    token_len = 0;
                }
            } else if (*p == '|' || *p == '>' || *p == '<') {
                if (current_token) {
                    // Finish current token first
                    current_token[token_len] = '\0';
                    char **new_tokens = realloc(tokens, sizeof(char*) * (count + 1));
                    if (!new_tokens) {
                        perror("realloc");
                        free(current_token);
                        for (int i = 0; i < count; i++) free(tokens[i]);
                        free(tokens);
                        return NULL;
                    }
                    tokens = new_tokens;
                    tokens[count++] = current_token;
                    current_token = NULL;
                    token_len = 0;
                    token_cap = 0;
                }
                
                // Handle operators
                char *op_token;
                if ((*p == '>' && *(p+1) == '>') || 
                    (*p == '<' && *(p+1) == '<')) {
                    op_token = malloc(3);
                    if (!op_token) {
                        perror("malloc");
                        for (int i = 0; i < count; i++) free(tokens[i]);
                        free(tokens);
                        return NULL;
                    }
                    op_token[0] = *p;
                    op_token[1] = *(p+1);
                    op_token[2] = '\0';
                    p++;
                } else {
                    op_token = malloc(2);
                    if (!op_token) {
                        perror("malloc");
                        for (int i = 0; i < count; i++) free(tokens[i]);
                        free(tokens);
                        return NULL;
                    }
                    op_token[0] = *p;
                    op_token[1] = '\0';
                }
                
                // Add operator token
                char **new_tokens = realloc(tokens, sizeof(char*) * (count + 1));
                if (!new_tokens) {
                    perror("realloc");
                    free(op_token);
                    for (int i = 0; i < count; i++) free(tokens[i]);
                    free(tokens);
                    return NULL;
                }
                tokens = new_tokens;
                tokens[count++] = op_token;
            } else {
                if (!current_token) {
                    current_token = malloc(32);
                    if (!current_token) {
                        perror("malloc");
                        for (int i = 0; i < count; i++) free(tokens[i]);
                        free(tokens);
                        return NULL;
                    }
                    token_cap = 32;
                    token_len = 0;
                }
                
                if (token_len + 1 >= token_cap) {
                    token_cap *= 2;
                    char *new_token = realloc(current_token, token_cap);
                    if (!new_token) {
                        perror("realloc");
                        free(current_token);
                        for (int i = 0; i < count; i++) free(tokens[i]);
                        free(tokens);
                        return NULL;
                    }
                    current_token = new_token;
                }
                current_token[token_len++] = *p;
            }
        }
    }

    // Handle any remaining token
    if (current_token) {
        current_token[token_len] = '\0';
        char **new_tokens = realloc(tokens, sizeof(char*) * (count + 1));
        if (!new_tokens) {
            perror("realloc");
            free(current_token);
            for (int i = 0; i < count; i++) free(tokens[i]);
            free(tokens);
            return NULL;
        }
        tokens = new_tokens;
        tokens[count++] = current_token;
    }

    // Check for unclosed quotes
    if (in_quote) {
        fprintf(stderr, "Error: unclosed quote\n");
        for (int i = 0; i < count; i++) free(tokens[i]);
        free(tokens);
        return NULL;
    }

    // Add NULL terminator
    char **new_tokens = realloc(tokens, sizeof(char*) * (count + 1));
    if (!new_tokens) {
        perror("realloc");
        for (int i = 0; i < count; i++) free(tokens[i]);
        free(tokens);
        return NULL;
    }
    tokens = new_tokens;
    tokens[count] = NULL;
    
    return tokens;
}
