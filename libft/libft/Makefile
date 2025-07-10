# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sholz <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/10 14:08:36 by sholz             #+#    #+#              #
#    Updated: 2024/11/10 14:08:38 by sholz            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC	= gcc
CFLAGS	= -Wall -Wextra -Werror
AR	= ar crs
RANLIB	= ranlib

NAME	= prelibft.a
HEAD	= libft.h
SRCS	= ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
	ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
	ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
	ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
	ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c

OBJS	= $(SRCS:.c=.o)


all: $(NAME)

$(NAME):
		@$(CC) $(CFLAGS) -c $(SRCS)
		@$(AR) $(NAME) $(OBJS)
		@$(RANLIB) $(NAME)
		#@echo "[INFO] Library [$(NAME)] created!"

clean:
		@rm -f $(OBJS)
		#@echo "[INFO] Objects removed!"

fclean: clean
		@rm -f $(NAME)
		#@echo "[INFO] Library [$(NAME)] removed!"

re:		fclean all

.PHONY: all, clean, fclean, re

so:
	$(CC) -nostartfiles -fPIC $(CFLAGS) $(SRCS)
	gcc -nostartfiles -shared -o libft.so $(OBJS)
