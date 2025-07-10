# **************************************************************************** #
#                                MINISHELL MAKEFILE                            #
# **************************************************************************** #

# Program Name
NAME := minishell

# Compiler & Flags
CC := cc
CFLAGS := -Wall -Wextra -Werror -I include -I libft/include

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
LIBFT_DIR := libft

# Files
SRC := $(shell find $(SRC_DIR) -name '*.c' ! -name '*_bonus.c')
BONUS_SRC := $(shell find $(SRC_DIR) -name '*_bonus.c')
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BONUS_OBJ := $(BONUS_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Libraries
LIBFT := $(LIBFT_DIR)/libft.a
LDLIBS := -L$(LIBFT_DIR) -lft -lreadline

# Default rule
all: $(NAME)

# Link objects into executable
$(NAME): $(LIBFT) $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OBJ) $(LDLIBS) -o $@

# Create object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile libft
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

# Bonus build
bonus: $(LIBFT) $(OBJ) $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(BONUS_OBJ) $(LDLIBS) -o $(NAME)

# Clean object files
clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

# Clean all build artifacts
fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re bonus
