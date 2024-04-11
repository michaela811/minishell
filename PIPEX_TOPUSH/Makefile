# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/23 10:36:52 by mmasarov          #+#    #+#              #
#    Updated: 2024/01/23 10:37:02 by mmasarov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
HEADER = pipex.h
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

LIBDIR = ./libft
LIBFT = $(LIBDIR)/libft.a

SOURCES = pipex.c \
			exec_functions.c \
			get_and_open.c \
			error_functions.c \
			libft/libft.a

OBJ = $(SOURCES:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< -I.

$(NAME): $(LIBFT) $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -L$(LIBDIR) -lft -o $(NAME)

$(LIBFT):
		$(MAKE) -C $(LIBDIR) all

.PHONY: clean fclean all re

clean:
		$(MAKE) -C $(LIBDIR) clean
		rm -f $(OBJ)

fclean: clean
		$(MAKE) -C $(LIBDIR) fclean
		rm -rf $(NAME)

re: fclean all
