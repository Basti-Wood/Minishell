NAME := minishell

CC := cc
CFLAGS := -Wall -Wextra -Werror -I include -I libft/include

SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
LIBFT_DIR := libft


SRC := $(shell find $(SRC_DIR) -name '*.c' ! -name '*_bonus.c')
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT := $(LIBFT_DIR)/prelibft.a
LDLIBS := -L$(LIBFT_DIR) -lft -lreadline


all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
