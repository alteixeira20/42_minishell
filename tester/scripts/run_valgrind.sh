#!/bin/bash

# General Files
MINISHELL=../minishell
TEST_FILE=./test_cases_auto.txt
RESULT_FILE=./results/valgrind_results.txt
LOG_DIR=./results/valgrind_logs
SUPPRESS=./readline.supp

# Color Codes
BOLD=$(tput bold)
GREEN=$(tput setaf 2)
RED=$(tput setaf 1)
ORANGE=$(tput setaf 214)
RESET=$(tput sgr0)
MAG=$(tput setaf 5)
SPACING="    "

mkdir -p "$LOG_DIR"
echo "" > "$RESULT_FILE"

echo "${MAG}${BOLD}[Tester]${RESET} Running tests with Valgrind..."
i=1
pass=0
fail=0
total=0

mapfile -t commands < <(grep -vE '^\s*#|^\s*$' "$TEST_FILE")

for cmd in "${commands[@]}"; do
	log_file="${LOG_DIR}/test_$i.log"

	valgrind --leak-check=full --error-exitcode=42 \
		--suppressions="$SUPPRESS" \
		--log-file="$log_file" --quiet \
		"$MINISHELL" <<< "$cmd"$'\nexit\n' > /dev/null 2>&1

	if [ $? -eq 0 ]; then
		echo -e "${SPACING} ${ORANGE}Command[$i]:${RESET} [$cmd] - ${GREEN}${BOLD}OK${RESET}."
		echo "[$i] OK  => $cmd" >> "$RESULT_FILE"
		pass=$((pass + 1))
	else
		echo -e "${SPACING} ${ORANGE}Command[$i]:${RESET} [$cmd] - ${RED}${BOLD}KO${RESET}"
		echo "[$i] KO  => $cmd" >> "$RESULT_FILE"
		fail=$((fail + 1))
	fi

	i=$((i + 1))
	total=$((total + 1))
done

echo ""
if [ $fail -eq 0 ]; then
	echo -e "${MAG}${BOLD}[Tester]${RESET}${GREEN}${BOLD} $total Tests Passed!${RESET} 🎉"
else
	echo -e "${MAG}${BOLD}[Tester]${RESET}${RED}${BOLD} $fail/$total Tests Failed.${RESET} Check ${BOLD}$RESULT_FILE${RESET} for more info."
fi
