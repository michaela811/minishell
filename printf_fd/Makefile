NAME = libprintf_fd.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -I.
AR = ar rcs

SRC = \
	ft_printf_fd.c \
	ft_print_char_fd.c \
	ft_print_str_fd.c \
	ft_print_int_fd.c \
	ft_print_unsign_int_fd.c \
	ft_print_hex_fd.c \
	ft_print_ptr_fd.c \
	ft_print_percent_fd.c

OBJ := $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $@ $^

%.o: %.c ft_printf_fd.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
