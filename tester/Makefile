# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/15 17:06:04 by paalexan          #+#    #+#              #
#    Updated: 2025/04/26 17:59:50 by paalexan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                              MiniShell - Tester                              #
# **************************************************************************** #

# Executables
MINISHELL_EXEC = $(abspath ../minishell)

# Directories
RESULTS_DIR				= results
MINI_DIR				= $(RESULTS_DIR)/minishell
BASH_DIR				= $(RESULTS_DIR)/bash
DIFF_DIR				= $(RESULTS_DIR)/diff
MINI_TMP				= $(MINI_DIR)/tmp_env
BASH_TMP				= $(BASH_DIR)/tmp_env
SCRIPTS_DIR				= scripts

# Scripts
TESTER_SCRIPT			= $(SCRIPTS_DIR)/set_script_env.sh
SUMMARY_SCRIPT			= $(SCRIPTS_DIR)/generate_summary_diff.py
RUN_MINISHELL			= $(SCRIPTS_DIR)/run_minishell.sh
RUN_BASH				= $(SCRIPTS_DIR)/run_bash.sh

# Rules
.PHONY: all run diff summary clean fclean help re

all: start_msg check_and_clean run diff summary

start_msg:
	@echo "$(MAG)$(PREFIX)$(RESET) $(BOLD)Running Tests: $(ORANGE)$(BOLD)MiniShell$(RESET)$(BOLD) VS $(ORANGE)bash$(RESET)\n"

re: fclean all

run:
	@mkdir -p $(MINI_TMP) $(BASH_TMP) $(DIFF_DIR)
	@$(RUN_MINISHELL) $(MINISHELL_EXEC) test_cases.txt $(MINI_TMP)
	@$(RUN_BASH) test_cases.txt $(BASH_TMP)

diff:
	@echo "$(MAG)$(PREFIX)$(RESET) $(BOLD)Comparing outputs of different $(ORANGE)commands$(RESET)..."
	@mkdir -p $(DIFF_DIR)
	@pass=0; fail=0; line_idx=0; test_idx=1; \
	while IFS= read -r line || [ -n "$$line" ]; do \
		line_idx=$$((line_idx+1)); \
		case "$$line" in \
			""|\#*) continue ;; \
		esac; \
		testname="test$$test_idx"; \
		mini_out="$(MINI_DIR)/$$testname.out"; \
		bash_out="$(BASH_DIR)/$$testname.out"; \
		diff_file="$(DIFF_DIR)/$$testname.diff"; \
		if [ -f "$$bash_out" ]; then \
			if diff -u "$$mini_out" "$$bash_out" > "$$diff_file"; then \
				echo "$(SPACING) $(ORANGE)Command[$$test_idx]:$(RESET) [$$line] - $(GREEN)$(BOLD)OK$(RESET)."; \
				rm -f "$$diff_file"; \
				pass=$$((pass+1)); \
			else \
				echo "$(SPACING) $(ORANGE)Command[$$test_idx]:$(RESET) [$$line] - $(RED)$(BOLD)KO$(RESET)"; \
				fail=$$((fail+1)); \
			fi; \
		else \
			echo "$(MAG)$(PREFIX)$(RESET) Missing bash output: $$bash_out"; \
			fail=$$((fail+1)); \
		fi; \
		test_idx=$$((test_idx+1)); \
	done < test_cases.txt; \
	total=$$((pass+fail)); \
	echo ""; \
	if [ $$fail -eq 0 ]; then \
		echo "$(MAG)$(PREFIX)$(GREEN) $(BOLD)$$total Tests Passed!$(RESET) 🎉"; \
	else \
		echo "$(MAG)$(PREFIX)$(RED) $(BOLD)$$fail/$$total Tests Failed.$(RESET) Check $(BOLD)diff_summary.txt$(RESET) for more info."; \
	fi

summary:
	@python3 $(SUMMARY_SCRIPT)

fclean: clean
	@rm -rf $(RESULTS_DIR)
	@rm -f diff_summary.txt
	@rm -rf $(MINI_DIR)/*.out $(BASH_DIR)/*.out $(DIFF_DIR)/*.diff
	@rm -rf $(MINI_TMP) $(BASH_TMP)
	@echo "$(MAG)$(PREFIX)$(RESET) Full cleanup done.\n"

check_and_clean:
	@if [ -d "$(RESULTS_DIR)" ] || [ -f "diff_summary.txt" ]; then \
		echo "$(MAG)$(PREFIX)$(RESET) Previous test results detected."; \
		$(MAKE) --silent fclean; \
	fi

help:
	@echo "$(MAG)$(BOLD)Available commands:$(RESET)"
	@echo "  $(BOLD)make run$(RESET)      - Run all test cases"
	@echo "  $(BOLD)make diff$(RESET)     - Compare outputs"
	@echo "  $(BOLD)make summary$(RESET)  - Generate a readable diff summary"
	@echo "  $(BOLD)make clean$(RESET)    - Clean outputs only"
	@echo "  $(BOLD)make fclean$(RESET)   - Full cleanup"
	@echo "  $(BOLD)make help$(RESET)     - Show this help message"

%:
	@echo "$(MAG)$(PREFIX)$(RESET) Rule not found. Use $(BOLD)\"make help\"$(RESET) for more info."

# Colors
PREFIX	= $(shell tput bold)[Tester]
SPACING	= "    "	
BOLD	= $(shell tput bold)
GREEN	= $(shell tput setaf 2)
RED		= $(shell tput setaf 1)
YELLOW	= $(shell tput setaf 3)
RESET	= $(shell tput sgr0)
GREY	= $(shell tput setaf 8)
ORANGE	= $(shell tput setaf 214)
MAG		= $(shell tput setaf 5)
