#!/bin/bash

MINISHELL=../minishell
TEST_FILE=./test_cases_auto.txt
RESULT_FILE=./results/valgrind_results
LOG_DIR=./results/valgrind_logs
SUPPRESS=./readline.supp

mkdir -p "$LOG_DIR"
echo "" > "$RESULT_FILE"

i=1
grep -vE '^\s*#|^\s*$' "$TEST_FILE" | while IFS= read -r cmd; do
	log_file="${LOG_DIR}/test_$i.log"

	valgrind --leak-check=full --error-exitcode=42 \
		--suppressions="$SUPPRESS" \
		--log-file="$log_file" --quiet \
		"$MINISHELL" <<< "$cmd"$'\nexit\n' > /dev/null 2>&1

	if [ $? -eq 0 ]; then
		echo -e "\tCommand[$i]: [$cmd] - OK."
		echo "[$i] OK  => $cmd" >> "$RESULT_FILE"
	else
		echo -e "\tCommand[$i]: [$cmd] - KO."
		echo "[$i] KO  => $cmd" >> "$RESULT_FILE"
	fi
	i=$((i + 1))
done
