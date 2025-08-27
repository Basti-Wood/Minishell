#!/bin/bash

# Test script to verify the fixes work correctly
# Run this after applying the fixes to ensure input redirections work

echo "Creating test files..."
mkdir -p test_files_debug
echo "hi" > test_files_debug/test1.txt
echo "hello world 42" > test_files_debug/test2.txt
echo "line 1" > test_files_debug/test3.txt
echo "line 2" >> test_files_debug/test3.txt
echo "line 3" >> test_files_debug/test3.txt

echo "Testing basic input redirection..."
echo "Test 1: cat < test_files_debug/test1.txt"
./minishell << 'EOF'
cat < test_files_debug/test1.txt
exit
EOF

echo -e "\nTest 2: cat <test_files_debug/test2.txt (no space)"
./minishell << 'EOF'
cat <test_files_debug/test2.txt
exit
EOF

echo -e "\nTest 3: cat <\"test_files_debug/test3.txt\" (with quotes)"
./minishell << 'EOF'
cat <"test_files_debug/test3.txt"
exit
EOF

echo -e "\nTest 4: grep line < test_files_debug/test3.txt"
./minishell << 'EOF'
grep line < test_files_debug/test3.txt
exit
EOF

echo -e "\nTest 5: Multiple redirections"
./minishell << 'EOF'
cat < test_files_debug/test1.txt < test_files_debug/test2.txt
exit
EOF

echo -e "\nCleanup test files..."
rm -rf test_files_debug

echo -e "\nIf all tests showed output without crashes, the fixes are working!"
