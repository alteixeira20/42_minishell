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
SRC_PATH		:= src
BUILD_PATH		:= .build
INIT_DIR		:= $(SRC_PATH)/200_inits
PARSER_DIR		:= $(SRC_PATH)/300_parser
TOKEN_DIR		:= $(SRC_PATH)/400_tokenizer
ENV_GET_DIR		:= $(SRC_PATH)/500_env_get
ENV_SET_DIR		:= $(SRC_PATH)/510_env_Set
BUILTINS_DIR	:= $(SRC_PATH)/610_builtins
ERROR_DIR		:= $(SRCSRC_PATH_DIR)/800_error
OBJ_DIR			:= obj

# Libft
LIBFT_REPO	:= https://github.com/alteixeira20/42_libft.git
LIBFT_DIR	:= libft
LIBFT		:= $(LIBFT_DIR)/libft.a

# Source Files
SRC			= $(SRC_PATH)/000_minishell.c
SRC			+= $(INIT_DIR)/init.c
SRC			+= $(PARSER_DIR)/parser_input_utils.c
SRC			+= $(PARSER_DIR)/parser_input.c
SRC			+= $(TOKEN_DIR)/parser_tokens.c
SRC			+= $(BUILTINS_DIR)/echo.c
SRC			+= $(ENV_GET_DIR)/env_get.C
SRC			+= $(ENV_SET_DIR)/env_set.c
SRC			+= $(BUILTINS_DIR)/utils.c
SRC			+= $(ERROR_DIR)/error.c

OBJS        = $(SRC:$(SRC_PATH)/%.c=$(BUILD_PATH)/%.o)

# Executable
MINISHELL	:= minishell

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

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c $(HEADERS) | $(BUILD_PATH)
	@$(MKDIR) $(@D)
	@printf "${CYAN}${DIM}Compiling: ${WHITE}%-35s${RESET}\r" "$(notdir $<)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_PATH):
	@printf "${BLUE}${BOLD}${BUILD} Creating build directory...${RESET}\n"
	@$(MKDIR) $(BUILD_PATH)
	@printf "${GREEN}${CHECK} Build directory ready${RESET}\n"

$(MINISHELL): $(OBJ) $(LIBFT) $(BUILD_PATH)
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
