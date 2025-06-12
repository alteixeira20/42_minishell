# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/03 02:10:25 by paalexan          #+#    #+#              #
#    Updated: 2025/06/12 22:40:42 by paalexan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# **************************************************************************** #
#                             Variables Declarion                              #
# **************************************************************************** #

### Executable
NAME			= minishell

### Repositories
LIBFT_REPO  	= git@github.com:alteixeira20/42_libft.git

### Message Vars
PREFIX 			= $(B)$(MAG)[MiniShell]$(D)
SUCCESSFULLY 	= $(GRN)successfully$(D)
NORM_SUCCESS 	= $(GRN)=== OK:$(D)
NORM_INFO 		= $(BLU)File no:$(D)
NORM_ERR 		= $(RED)=== KO:$(D)
SEP 			= =====================

# **************************************************************************** #
#                                 Directories                                  #
# **************************************************************************** #

LIBFT_DIR		= libft
LIBFT			= $(LIBFT_DIR)/libft.a

SRC_DIR 		= src
INIT_DIR		= $(SRC_DIR)/inits
SIGNAL_DIR		= $(SRC_DIR)/signals
PARSER_DIR		= $(SRC_DIR)/parser
TOKEN_DIR		= $(SRC_DIR)/tokenizer
ENV_DIR			= $(SRC_DIR)/env
EXEC_DIR		= $(SRC_DIR)/execution
BUILTINS_DIR	= $(SRC_DIR)/builtins
REDIRECTS_DIR	= $(SRC_DIR)/redirection
ERROR_DIR		= $(SRC_DIR)/error
CLEANUP_DIR		= $(SRC_DIR)/cleanup

TEMP_DIR		= .temp
OBJ_DIR			= .obj

# **************************************************************************** #
#                                 Source Files                                 #
# **************************************************************************** #

SRC				= $(SRC_DIR)/main.c
SRC				+= $(INIT_DIR)/init.c
SRC				+= $(INIT_DIR)/init_2.c
SRC				+= $(SIGNAL_DIR)/signals.c
SRC				+= $(PARSER_DIR)/parser.c
SRC				+= $(PARSER_DIR)/parser_syntax.c
SRC				+= $(PARSER_DIR)/parser_utils.c
SRC				+= $(PARSER_DIR)/parser_expansion.c
SRC				+= $(TOKEN_DIR)/tokenizer.c
SRC				+= $(TOKEN_DIR)/tokenizer_split.c
SRC				+= $(TOKEN_DIR)/tokenizer_utils.c
SRC				+= $(TOKEN_DIR)/tokenizer_lst.c
SRC				+= $(EXEC_DIR)/exec.c
SRC				+= $(EXEC_DIR)/exec_builtin_parent.c
SRC				+= $(EXEC_DIR)/exec_pipes.c
SRC				+= $(EXEC_DIR)/exec_child.c
SRC				+= $(EXEC_DIR)/exec_child_utils.c
SRC				+= $(EXEC_DIR)/exec_cmd.c
SRC				+= $(EXEC_DIR)/exec_signals.c
SRC				+= $(EXEC_DIR)/exec_utils.c
SRC				+= $(ENV_DIR)/env_get.c
SRC				+= $(ENV_DIR)/env_set.c
SRC				+= $(ENV_DIR)/env_utils.c
SRC				+= $(BUILTINS_DIR)/builtin_echo.c
SRC				+= $(BUILTINS_DIR)/builtin_export.c
SRC				+= $(BUILTINS_DIR)/builtin_export_utils.c
SRC				+= $(BUILTINS_DIR)/builtin_env.c
SRC				+= $(BUILTINS_DIR)/builtin_pwd.c
SRC				+= $(BUILTINS_DIR)/builtin_cd.c
SRC				+= $(BUILTINS_DIR)/builtin_unset.c
SRC				+= $(BUILTINS_DIR)/builtin_exit_utils.c
SRC				+= $(BUILTINS_DIR)/builtin_exit.c
SRC				+= $(BUILTINS_DIR)/builtins.c
SRC				+= $(BUILTINS_DIR)/builtins_utils.c
SRC				+= $(BUILTINS_DIR)/prompt_utils.c
SRC				+= $(REDIRECTS_DIR)/redirects.c
SRC				+= $(REDIRECTS_DIR)/redirects_apply.c
SRC				+= $(REDIRECTS_DIR)/redirects_exec.c
SRC				+= $(REDIRECTS_DIR)/redirects_parser.c
SRC				+= $(REDIRECTS_DIR)/redirects_check.c
SRC				+= $(REDIRECTS_DIR)/heredocs_utils.c
SRC				+= $(REDIRECTS_DIR)/heredocs_utils_2.c
SRC				+= $(REDIRECTS_DIR)/heredocs.c
SRC				+= $(ERROR_DIR)/error.c
SRC				+= $(CLEANUP_DIR)/cleanup_general.c
SRC				+= $(CLEANUP_DIR)/cleanup_cmd.c
SRC				+= $(CLEANUP_DIR)/cleanup_heredoc.c

# **************************************************************************** #
#                                   Targets                                    #
# **************************************************************************** #

OBJS			= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

#==============================================================================#
#                              COMPILER & FLAGS                                #
#==============================================================================#

CC			= cc
CFLAGS		= -Wall -Werror -Wextra
DFLAGS		= -g

#==============================================================================#
#                                COMMANDS                                      #
#==============================================================================#

MAKE		= make -C

all: $(LIBFT) $(NAME)

$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		echo "$(PREFIX) Cloning $(BOLD)Libft$(RESET) and waiting for compilation..."; \
		git clone $(LIBFT_REPO) $(LIBFT_DIR) > /dev/null 2>&1; \
	fi
	@$(MAKE) $(LIBFT_DIR) --silent > /dev/null 2>&1
	@echo "$(PREFIX) $(BOLD)Libft$(RESET) compiled $(SUCCESSFULLY)."

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_DIR) $(LIBFT) $(OBJS) 
	@$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "$(PREFIX) $(BOLD)Executable$(RESET) compiled $(SUCCESSFULLY)."

valgrind:
	@if [ ! -f "$(NAME)" ]; then \
		echo "$(PREFIX) Executable not found. Running 'make'..."; \
		make --silent; \
	fi
	@echo "{\n readline leaks\n   Memcheck:Leak\n...\n   fun:readline\n}\n{\n   leak add_history\n   Memcheck:Leak\n...\n   fun:add_history\n}" > readline.supp
	@valgrind --suppressions=readline.supp --leak-check=full -s --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes	--child-silent-after-fork=no ./$(NAME)
	@rm -f readline.supp

norm:
	@echo "$(PREFIX) Running norminette on source files...$(D)"
	@norminette $(SRC) | while read -r line; do \
		if echo $$line | grep -q "Error"; then \
			echo "$(NORM_ERR) $$line"; \
		elif echo $$line | grep -q "OK!"; then \
			echo "$(NORM_SUCCESS) $$line"; \
		else \
			echo "$(NORM_INFO) $$line"; \
		fi; \
	done

clean:
	@if [ -d "$(OBJ_DIR)" ] || [ -f "$(NAME)" ]; then \
		rm -rf $(OBJ_DIR) $(NAME); \
		echo "$(PREFIX) Objects & Executable removed $(SUCCESSFULLY)"; \
	else \
		echo "$(PREFIX) No Objects & Executable to clean."; \
	fi
	@rm -f readline.supp

fclean:
	@make clean --silent
	@if [ -d "$(LIBFT_DIR)" ]; then \
		rm -rf $(LIBFT_DIR); \
		echo "$(PREFIX) Libft removed $(SUCCESSFULLY)"; \
	else \
		echo "$(PREFIX) No Libft folder to clean."; \
	fi

re: fclean all

help:
	@awk 'BEGIN {FS = ":.*##"; \
			printf "\n=> Usage:\n\tmake $(GRN)<target>$(D)\n"} \
		/^[a-zA-Z_0-9-]+:.*?##/ { \
			printf "\t$(GRN)%-15s$(D) %s\n", $$1, $$2 } \
		/^##@/ { \
			printf "\n=> %s\n", substr($$0, 5) } ' Makefile
## Tweaked from source:
### https://www.padok.fr/en/blog/beautiful-makefile-awk

.PHONY: clean fclean re norm

#==============================================================================#
#                                  UTILS                                       #
#==============================================================================#

# Colors
#
# Run the following command to get list of available colors
# bash -c 'for c in {0..255}; do tput setaf $c; tput setaf $c | cat -v; echo =$c; done'
#
B  		= $(shell tput bold)
BLA		= $(shell tput setaf 0)
RED		= $(shell tput setaf 1)
GRN		= $(shell tput setaf 2)
YEL		= $(shell tput setaf 3)
BLU		= $(shell tput setaf 4)
MAG		= $(shell tput setaf 5)
CYA		= $(shell tput setaf 6)
WHI		= $(shell tput setaf 7)
GRE		= $(shell tput setaf 8)
BRED 	= $(shell tput setaf 9)
BGRN	= $(shell tput setaf 10)
BYEL	= $(shell tput setaf 11)
BBLU	= $(shell tput setaf 12)
BMAG	= $(shell tput setaf 13)
BCYA	= $(shell tput setaf 14)
BWHI	= $(shell tput setaf 15)
D 		= $(shell tput sgr0)
BEL 	= $(shell tput bel)
CLR 	= $(shell tput el 1)
