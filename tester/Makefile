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

all: check_and_clean run diff summary

re: fclean all

run:
	@echo "$(ORANGE)$(PREFIX)$(RESET) Running Output Tests..."
	@mkdir -p $(MINI_TMP) $(BASH_TMP) $(DIFF_DIR)
	@$(RUN_MINISHELL) $(MINISHELL_EXEC) test_cases.txt $(MINI_TMP)
	@$(RUN_BASH) test_cases.txt $(BASH_TMP)

diff:
	@echo "$(ORANGE)$(PREFIX)$(RESET) Comparing outputs..."
	@mkdir -p $(DIFF_DIR)
	@pass=0; fail=0; \
	for f in $$(ls -v $(MINI_DIR)/test*.out); do \
		testname=$$(basename $$f .out); \
		bash_file="$(BASH_DIR)/$$testname.out"; \
		diff_file="$(DIFF_DIR)/$$testname.diff"; \
		if [ -f "$$bash_file" ]; then \
			if diff -u "$$f" "$$bash_file" > "$$diff_file"; then \
				echo "$(ORANGE)$(PREFIX)$(RESET) $$testname: $(GREEN)OK$(RESET)."; \
				rm -f "$$diff_file"; \
				pass=$$((pass+1)); \
			else \
				echo "$(ORANGE)$(PREFIX)$(RESET) $$testname: $(RED)KO$(RESET). Check $(BOLD)$$diff_file$(RESET) for more info."; \
				fail=$$((fail+1)); \
			fi; \
		else \
			echo "$(ORANGE)$(PREFIX)$(RESET) Missing bash output: $$bash_file"; \
			fail=$$((fail+1)); \
		fi; \
	done; \
	total=$$((pass+fail)); \
	echo ""; \
	if [ $$fail -eq 0 ]; then \
		echo "$(ORANGE)$(PREFIX)$(GREEN) $(BOLD)$$total Tests Passed!$(RESET) 🎉"; \
	else \
		echo "$(ORANGE)$(PREFIX)$(RED) $(BOLD)$$fail/$$total Tests Failed.$(RESET) Check $(BOLD)$(DIFF_DIR)/$(RESET) for more info."; \
	fi

summary:
	@echo "$(ORANGE)$(PREFIX)$(RESET) Generating diff summary..."
	@python3 $(SUMMARY_SCRIPT) > diff_summary.txt && \
	echo "$(ORANGE)$(PREFIX)$(RESET) Summary written to $(BOLD)diff_summary.txt$(RESET)"

clean:
	@rm -rf $(MINI_DIR)/*.out $(BASH_DIR)/*.out $(DIFF_DIR)/*.diff
	@rm -rf $(MINI_TMP) $(BASH_TMP)
	@echo "$(ORANGE)$(PREFIX)$(RESET) Cleaned test outputs."

fclean: clean
	@rm -rf $(RESULTS_DIR)
	@rm -f diff_summary.txt
	@echo "$(ORANGE)$(PREFIX)$(RESET) Full cleanup done."

check_and_clean:
	@if [ -d "$(RESULTS_DIR)" ] || [ -f "diff_summary.txt" ]; then \
		echo "$(ORANGE)$(PREFIX)$(RESET) Previous test results detected."; \
		$(MAKE) --silent fclean; \
	fi

help:
	@echo "$(ORANGE)$(BOLD)Available commands:$(RESET)"
	@echo "  $(BOLD)make run$(RESET)      - Run all test cases"
	@echo "  $(BOLD)make diff$(RESET)     - Compare outputs"
	@echo "  $(BOLD)make summary$(RESET)  - Generate a readable diff summary"
	@echo "  $(BOLD)make clean$(RESET)    - Clean outputs only"
	@echo "  $(BOLD)make fclean$(RESET)   - Full cleanup"
	@echo "  $(BOLD)make help$(RESET)     - Show this help message"

%:
	@echo "$(ORANGE)$(PREFIX)$(RESET) Rule not found. Use $(BOLD)\"make help\"$(RESET) for more info."

# Colors
PREFIX := $(shell tput bold)[Tester]
BOLD   := $(shell tput bold)
GREEN  := $(shell tput setaf 2)
RED    := $(shell tput setaf 1)
YELLOW := $(shell tput setaf 3)
RESET  := $(shell tput sgr0)
GREY   := $(shell tput setaf 8)
ORANGE := $(shell tput setaf 214)
