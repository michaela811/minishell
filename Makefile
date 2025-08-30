NAME = minishell
CC = cc
CFLAGS = -g -Wall -Wextra -Werror -MMD -MP # -fsanitize=address

INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj

LIBDIR = ./libft
LIBFT = $(LIBDIR)/libft.a

PRINTFD_DIR := ./printf_fd
PRINTFD_LIB := $(PRINTFD_DIR)/libprintf_fd.a

# -------- Readline (Linux/macOS) --------
UNAME_S   := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  RL_INC  := -I/opt/homebrew/opt/readline/include
  RL_LIB  := -L/opt/homebrew/opt/readline/lib -lreadline
else
  RL_INC  := 
  RL_LIB  := -lreadline
endif

SRC = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
DEP = $(OBJ:.o=.d)

.PHONY: all clean fclean re libs

all: $(NAME)

$(NAME): $(LIBFT) $(PRINTFD_LIB) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(PRINTFD_LIB) -L$(LIBDIR) -lft $(RL_LIB) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBDIR) -I$(PRINTFD_DIR) $(RL_INC) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBDIR)/.git:
	@git submodule update --init --recursive

$(LIBFT): | $(LIBDIR)/.git
	$(MAKE) -C $(LIBDIR) all

$(PRINTFD_LIB):
	$(MAKE) -C $(PRINTFD_DIR) all

clean:
	$(MAKE) -C $(LIBDIR) clean
	$(MAKE) -C $(PRINTFD_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	$(MAKE) -C $(PRINTFD_DIR) fclean
	rm -f $(NAME)

re: fclean all

-include $(DEP)
