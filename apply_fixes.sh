#!/bin/bash

# Script to apply the fixes to your minishell
# Run this script from the minishell root directory

echo "Applying fixes for double free and input redirection issues..."

# Backup original files
echo "Creating backups..."
cp src/Parsing/parser_redir.c src/Parsing/parser_redir.c.backup 2>/dev/null
cp src/excecute/exec_helper_2.c src/excecute/exec_helper_2.c.backup 2>/dev/null
cp src/excecute/quote_utiles.c src/excecute/quote_utiles.c.backup 2>/dev/null
cp src/utils/free_cmds.c src/utils/free_cmds.c.backup 2>/dev/null

# Apply the fixed files
echo "Applying parser_redir fix..."
cp src/Parsing/parser_redir_fixed.c src/Parsing/parser_redir.c

echo "Applying exec_helper_2 fix..."
cp src/excecute/exec_helper_2_fixed.c src/excecute/exec_helper_2.c

echo "Applying quote utils fix..."
if [ -f src/excecute/quote_utiles.c ]; then
    cp src/excecute/quote_utils_fixed.c src/excecute/quote_utiles.c
else
    echo "Warning: Could not find quote_utiles.c - you'll need to apply the remove_quote_markers fix manually"
fi

echo "Applying free_cmds fix..."
cp src/utils/free_cmds_fixed.c src/utils/free_cmds.c

echo ""
echo "IMPORTANT: You also need to:"
echo "1. Update your minishell.h to remove 'infiles' and 'outfiles' from the t_cmd structure"
echo "2. Remove any references to add_to_infiles and add_to_outfiles functions"
echo "3. Update any other files that use cmd->infiles or cmd->outfiles"
echo ""
echo "After making these changes:"
echo "  make re"
echo "  ./test_input_redir.sh  (to test the fixes)"
echo ""
echo "Original files backed up with .backup extension"
