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

OBJDIR = obj_printf_fd
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

all: $(OBJDIR) $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJ)
	$(AR) $@ $^

$(OBJDIR)/%.o: %.c ft_printf_fd.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
