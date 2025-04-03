# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/03 02:10:25 by paalexan          #+#    #+#              #
#    Updated: 2025/04/03 03:07:29 by paalexan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler & Flags
CC			:= cc
CFLAGS		:= -Wall -Werror -Wextra -g

# Directories
SRC_DIR		:= src
PARSER_DIR	:= $(SRC_DIR)/300_parser
TOKEN_DIR	:= $(SRC_DIR)/400_tokenizer
BUILTINS_DIR	:= $(SRC_DIR)/610_builtins
OBJ_DIR		:= obj

# Libft
LIBFT_REPO	:= https://github.com/alteixeira20/42_libft.git
LIBFT_DIR	:= libft
LIBFT		:= $(LIBFT_DIR)/libft.a

# Source Files
SRC			= $(SRC_DIR)/000_minishell.c
SRC			+= $(PARSER_DIR)/parser_input_utils.c
SRC			+= $(PARSER_DIR)/parser_input.c
SRC			+= $(TOKEN_DIR)/parser_tokens.c
SRC			+= $(BUILTINS_DIR)/cmd_echo.c
SRC			+= $(BUILTINS_DIR)/cmd_utils.c

OBJ			:= $(SRC:.c=.o)
OBJ			:= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Executable
MINISHELL	:= minishell

# Targets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(PARSER_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(CMDS_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Rules

.PHONY: all clean fclean re

all: $(LIBFT) $(MINISHELL)

$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		echo "$(ORANGE)$(PREFIX)$(RESET) Cloning $(BOLD)Libft$(RESET) and waiting for compilation..."; \
		git clone $(LIBFT_REPO) $(LIBFT_DIR) > /dev/null 2>&1; \
	fi
	@$(MAKE) -C $(LIBFT_DIR) --silent > /dev/null 2>&1
	@echo "$(ORANGE)$(PREFIX)$(RESET) $(BOLD)Libft$(RESET) compiled $(GREEN)successfully$(RESET)."

$(MINISHELL): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $(MINISHELL)
	@echo "$(ORANGE)$(PREFIX)$(RESET) $(BOLD)MiniShell$(RESET) compiled $(GREEN)successfully$(RESET)."

re: fclean all

clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(MINISHELL)
	@echo "$(ORANGE)$(PREFIX)$(RESET) All executables and objects were cleaned $(GREEN)successfully$(RESET)."

fclean: clean
	@rm -rf $(LIBFT_DIR)
	@echo "$(ORANGE)$(PREFIX)$(RESET) Libft was cleaned $(GREEN)successfully$(RESET)."

# Colors
PREFIX	:= $(shell tput bold)[So_Long]
BOLD 	:= $(shell tput bold)
GREEN	:= $(shell tput setaf 2)
RED  	:= $(shell tput setaf 1)
YELLOW	:= $(shell tput setaf 3)
RESET	:= $(shell tput sgr0)
GREY	:= $(shell tput setaf 8)
ORANGE	:= $(shell tput setaf 214)
