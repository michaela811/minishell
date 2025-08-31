# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dpadenko <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/15 16:23:59 by dpadenko          #+#    #+#              #
#    Updated: 2023/09/15 16:46:46 by dpadenko         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Werror -Wextra
NAME = libft.a
HEADER = libft.h

SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
	  ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c \
	  ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c \
	  ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c \
	  ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
	  ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c \
	  ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
	  ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
	  ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	  ft_strtok.c ft_strcmp.c ft_isspace.c ft_isnum.c \
	  ft_atof.c ft_strcpy.c ft_strpbrk.c ft_exact_strcmp.c \
	  ft_realloc.c ft_strcat.c ft_strncpy.c ft_strndup.c

OBJDIR = obj_libft
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
		ar rcs $@ $^

$(OBJDIR)/%.o: %.c $(HEADER)
		@mkdir -p $(OBJDIR)
		$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean fclean all re

clean: 
		rm -rf $(OBJDIR)
fclean: clean
		rm -rf $(NAME)

re: fclean all
