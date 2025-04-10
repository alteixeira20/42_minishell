# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/03 02:10:25 by paalexan          #+#    #+#              #
#    Updated: 2025/04/10 16:22:53 by paalexan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#==============================================================================#
#                                     NAMES                                    #
#==============================================================================#

NAME			:= minishell

### Message Vars
_PREFIX 		= $(MAG)[MiniShell]$(D)
_SUCCESS 		= $(GRN)[SUCCESS]$(D)
_INFO 			= $(BLU)[INFO]$(D)
_NORM 			= $(MAX)[Norminette]$(D)
_NORM_SUCCESS 	= $(GRN)=== OK:$(D)
_NORM_INFO 		= $(BLU)File no:$(D)
_NORM_ERR 		= $(RED)=== KO:$(D)
_SEP 			= =====================

#==============================================================================#
#                                    PATHS                                     #
#==============================================================================#

SRC_PATH 		= src
LIBS_PATH		= lib
BUILD_PATH		= .build
TEMP_PATH		= .temp

INIT_DIR		= $(SRC_PATH)/200_inits
PARSER_DIR		= $(SRC_PATH)/300_parser
TOKEN_DIR		= $(SRC_PATH)/400_tokenizer
ENV_GET_DIR		= $(SRC_PATH)/500_env_get
ENV_SET_DIR		= $(SRC_PATH)/510_env_set
EXEC_DIR		= $(SRC_PATH)/600_exec
BUILTINS_DIR	= $(SRC_PATH)/610_builtins
ERROR_DIR		= $(SRC_PATH)/800_error
FREE_DIR		= $(SRC_PATH)/900_free

SRC				= $(SRC_PATH)/000_minishell.c
SRC				+= $(INIT_DIR)/init.c
SRC				+= $(PARSER_DIR)/parser_input_utils.c
SRC				+= $(PARSER_DIR)/parser_input.c
SRC				+= $(TOKEN_DIR)/parser_tokens.c
SRC				+= $(EXEC_DIR)/exec.c
SRC				+= $(EXEC_DIR)/exec_utils.c
SRC				+= $(BUILTINS_DIR)/cmd_echo.c
SRC				+= $(BUILTINS_DIR)/cmd_export.c
SRC				+= $(BUILTINS_DIR)/cmd_env.c
SRC				+= $(BUILTINS_DIR)/cmd_cd.c
SRC				+= $(BUILTINS_DIR)/redirects.c
SRC				+= $(BUILTINS_DIR)/redirects_utils.c
SRC				+= $(BUILTINS_DIR)/cmd_utils.c
SRC				+= $(BUILTINS_DIR)/builtin_utils.c
SRC				+= $(BUILTINS_DIR)/prompt_utils.c
SRC				+= $(ENV_GET_DIR)/env_get.c
SRC				+= $(ENV_SET_DIR)/env_set.c
SRC				+= $(ERROR_DIR)/error.c
SRC				+= $(FREE_DIR)/free_utils.c


OBJS			= $(SRC:$(SRC_PATH)/%.c=$(BUILD_PATH)/%.o)

LIBFT_PATH		= $(LIBS_PATH)/libft
LIBFT_ARC		= $(LIBFT_PATH)/libft.a

#==============================================================================#
#                              COMPILER & FLAGS                                #
#==============================================================================#

CC			= cc

CFLAGS		= -Wall -Werror -Wextra
DFLAGS		= -g

INC			= -I

#==============================================================================#
#                                COMMANDS                                      #
#==============================================================================#

AR			= ar rcs
RM			= rm -rf
MKDIR_P		= mkdir -p

MAKE		= make -C

all: deps $(NAME)

bonus: deps $(NAME_BONUS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	@$(MKDIR_P) $(@D)
	@echo -n "$(MAG)█$(D)"
	@$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(BUILD_PATH):
	@$(MKDIR_P) $(BUILD_PATH)
	@echo "$(_PREFIX) $(YEL)Creating $(BUILD_PATH) folder:$(D) $(_SUCCESS)"
	@echo "$(_PREFIX) $(YEL)Compiling Minishell$(D)"

$(TEMP_PATH):
	@$(MKDIR_P) $(TEMP_PATH)
	@echo "$(_PREFIX) $(YEL)Creating $(TEMP_PATH) folder:$(D) $(_SUCCESS)"

$(NAME): $(BUILD_PATH) $(LIBFT_ARC) $(OBJS) 
	@$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) $(LIBFT_ARC) -lreadline -o $(NAME)
	@echo "\n$(_PREFIX) Compilation was $(BGRN)successful$(D)."

$(LIBFT_ARC):
	$(MAKE) $(LIBFT_PATH)

deps: 			## Download/Update libft
	@if test ! -d "$(LIBFT_PATH)"; then make get_libft; \
		else echo "$(_PREFIX) $(BYEL)Libft$(D) folder found 🖔"; fi
	@echo "$(_PREFIX) Nothing to be done!"


update_modules:
	@echo "* $(CYA)Updating submodules$(D)]"
	@git submodule init
	@git submodule update --recursive --remote
	@echo "* $(GRN)Submodules update$(D): $(_SUCCESS)"

get_libft:
	@echo "* $(CYA)Getting Libft submodule$(D)]"
	@git clone git@github.com:alteixeira20/42_libft.git $(LIBFT_PATH)
	@echo "* $(GRN)Libft submodule download$(D): $(_SUCCESS)"

clean: 				## Remove object files
	@echo "* $(YEL)Removing $(MAG)$(NAME)$(D) and deps $(YEL)object files$(D)"
	@if [ -d "$(LIBFT_PATH)/$(BUILD_PATH)" ] || [ -d "$(BUILD_PATH)" ] || [ -d "$(TEMP_PATH)" ]; then \
		if [ -d "$(LIBFT_PATH)/$(BUILD_PATH)" ]; then \
			$(MAKE) $(LIBFT_PATH) clean; \
			echo "$(_PREFIX) Removing $(CYA)libft$(D) object files$(D): $(_SUCCESS)"; \
		fi; \
		if [ -d "$(BUILD_PATH)" ]; then \
			$(RM) $(BUILD_PATH); \
			echo "$(_PREFIX) Removing $(CYA)$(BUILD_PATH)$(D) folder & files$(D): $(_SUCCESS)"; \
		fi; \
		if [ -d "$(BUILDB_PATH)" ]; then \
			$(RM) $(BUILDB_PATH); \
			echo "$(_PREFIX) Removing $(CYA)$(BUILDB_PATH)$(D) folder & files$(D): $(_SUCCESS)"; \
		fi; \
		if [ -d "$(TEMP_PATH)" ]; then \
			$(RM) $(TEMP_PATH); \
			echo "*$(_PREFIX) Removing $(CYA)$(TEMP_PATH)$(D) folder & files:$(D) $(_SUCCESS)"; \
		fi; \
	else \
		echo "$(_PREFIX) Nothing to clean!"; \
	fi

valgrind:
	@echo "{\n readline leaks\n   Memcheck:Leak\n...\n   fun:readline\n}\n{\n   leak add_history\n   Memcheck:Leak\n...\n   fun:add_history\n}" > readline.supp
	/usr/bin/valgrind --suppressions=readline.supp --leak-check=full -s --show-leak-kinds=all ./$(NAME)
	@rm -f readline.supp

fclean: clean	## Remove archives & executables
	$(RM) $(NAME) $(NAME_BONUS)
	@echo "* $(YEL)Cleaning executable$(D): $(_SUCCESS)"
	@rm -rf $(LIBFT_PATH)
	@echo "* $(YEL)Removing libft archive$(D): $(_SUCCESS)"

libclean: fclean	## Remove libs
	$(RM) $(LIBS_PATH)
	@echo "* $(YEL)Removing lib folder & files!$(D) : $(_SUCCESS)"

re: fclean all	## Purge and Recompile

help: 			## Display this help page
	@awk 'BEGIN {FS = ":.*##"; \
			printf "\n=> Usage:\n\tmake $(GRN)<target>$(D)\n"} \
		/^[a-zA-Z_0-9-]+:.*?##/ { \
			printf "\t$(GRN)%-15s$(D) %s\n", $$1, $$2 } \
		/^##@/ { \
			printf "\n=> %s\n", substr($$0, 5) } ' Makefile
## Tweaked from source:
### https://www.padok.fr/en/blog/beautiful-makefile-awk

.PHONY: deps get_libft update_modules clean fclean libclean re \
		norm

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
