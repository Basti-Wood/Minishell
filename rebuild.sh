#!/bin/bash

echo "Rebuilding minishell with memory fixes..."
cd "D:/twitchstuff/code stuff/Minishell"

# Clean and rebuild
make fclean
make

echo "Build complete! Now you can run your tests again."
echo "The main fixes applied:"
echo "1. Added free_redirs(tmp->redirs) in free_cmds.c to fix memory leak"
echo "2. Added missing free(line) calls in minishell.c to prevent leaks when tokenization fails"
