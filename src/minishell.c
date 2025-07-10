#include "../include/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1); 
	rl_replace_line("", 0);   
	rl_on_new_line();     
	rl_redisplay();  
}

void minishell(void)
{
    t_shell shell;
    char *line;
    t_token *tokens;
    t_token *tmp;
    t_cmd *cmds;
    t_cmd *cmd_tmp;
    int i;

    while (1)
    {
        line = readline("minishell> ");
        if (!line)
        {
            printf("exit\n");
            break;
        }

        if (*line)
            add_history(line);

        shell.input = line;
        tokens = tokenize(&shell);

        printf("\n--- Tokens ---\n");
        tmp = tokens;
        while (tmp)
        {
            printf("[%d] \"%s\"\n", tmp->type, tmp->str);
            tmp = tmp->next;
        }

        // Parse the tokens into commands
        cmds = parse_tokens(tokens);

        printf("\n--- Parsed Commands ---\n");
        cmd_tmp = cmds;
        while (cmd_tmp)
        {
            printf("Command:\n");
            if (cmd_tmp->argv)
            {
                i = 0;
                while (cmd_tmp->argv[i])
                {
                    printf("  argv[%d]: %s\n", i, cmd_tmp->argv[i]);
                    i++;
                }
            }

            if (cmd_tmp->infile)
                printf("  input  : %s\n", cmd_tmp->infile);
            if (cmd_tmp->outfile)
                printf("  output : %s (%s)\n", cmd_tmp->outfile, cmd_tmp->append ? "append" : "truncate");

            printf("\n");
            cmd_tmp = cmd_tmp->next;
        }

        // Free commands
        while (cmds)
        {
            cmd_tmp = cmds;
            cmds = cmds->next;
            if (cmd_tmp->argv)
            {
                i = 0;
                while (cmd_tmp->argv[i])
                    free(cmd_tmp->argv[i++]);
                free(cmd_tmp->argv);
            }
            free(cmd_tmp->infile);
            free(cmd_tmp->outfile);
            free(cmd_tmp);
        }

        // Free tokens
        while (tokens)
        {
            tmp = tokens;
            tokens = tokens->next;
            free(tmp->str);
            free(tmp);
        }

        free(line);
    }
}
