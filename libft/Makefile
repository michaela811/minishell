# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/13 16:14:04 by mmasarov          #+#    #+#              #
#    Updated: 2024/05/21 11:26:49 by dpadenko         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -g -Wall -Wextra -Werror
NAME = libft.a
HEADER = libft.h

SOURCES = ft_isdigit.c \
		  ft_isalnum.c \
		  ft_isascii.c \
		  ft_isprint.c \
		  ft_memset.c \
		  ft_bzero.c \
		  ft_memcpy.c \
		  ft_memmove.c \
		  ft_strlcpy.c \
		  ft_strlcat.c \
		  ft_toupper.c \
		  ft_tolower.c \
		  ft_strchr.c \
		  ft_strrchr.c \
		  ft_strncmp.c \
		  ft_memchr.c \
		  ft_memcmp.c \
		  ft_strnstr.c \
		  ft_atoi.c \
		  ft_calloc.c \
		  ft_strdup.c \
		  ft_substr.c \
		  ft_strtrim.c \
		  ft_split.c \
		  ft_itoa.c \
		  ft_strmapi.c \
		  ft_striteri.c \
		  ft_putchar_fd.c \
		  ft_putstr_fd.c \
		  ft_putendl_fd.c \
		  ft_putnbr_fd.c \
		  ft_isalpha.c \
		  ft_strpbrk.c \
		  ft_strtok.c \
		  ft_printf.c \
		  ft_print_char.c \
		  ft_print_hex.c \
		  ft_print_ptr.c \
		  ft_print_str.c \
		  ft_print_unsign_int.c \
		  ft_print_percent.c \
		  ft_print_int.c \
		  ft_printf_fd.c \
		  ft_print_char_fd.c \
		  ft_print_hex_fd.c \
		  ft_print_ptr_fd.c \
		  ft_print_str_fd.c \
		  ft_print_unsign_int_fd.c \
		  ft_print_percent_fd.c \
		  ft_print_int_fd.c \
		  ft_strcat.c \
		  ft_strcmp.c \
		  ft_strcpy.c \
		  ft_strndup.c \
		  ft_strncpy.c \
		  ft_strncat.c \
		  get_next_line.c \
		  get_next_line_utils.c \
		  ft_strlen.c \
		  ft_strjoin.c \

OBJECT = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECT)
		ar rcs $@ $^

%.o: %.c $(HEADER)
		$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean fclean re all

clean:
		rm -rf $(OBJECT)

fclean:		clean
		rm -rf $(NAME)

re: fclean $(NAME)
