#!/bin/bash

MINISHELL=$1
TEST_FILE=$2
TMP_ENV=$3
OUT_DIR=$(dirname "$TMP_ENV")

mkdir -p "$OUT_DIR"

i=1
grep -vE '^\s*#|^\s*$' "$TEST_FILE" | while IFS= read -r line; do
	outfile="${OUT_DIR}/test$i.out"
	echo "__CMD_START__" > "$outfile"

	if [[ "$line" != "env" ]]; then
		sed_clean=(
			-e '/^\w+@.*\$ .*/d'
			-e '/^exit$/d'
			-e 's/^(minishell|bash): //'
			-e 's/^declare -x OLDPWD=.*/declare -x OLDPWD/'
			-e 's/\x1B\[[0-9;]*[A-Za-z]//g'
			-e '/^declare -x _=.*$/d'
			-e '/^declare -x _P9K_TTY=.*$/d'
			-e 's/^declare -x SHLVL=.*/declare -x SHLVL=42/'
			-e 's/^minishell> //'
		)
		echo "$line" | "$MINISHELL" 2>&1 | sed -E "${sed_clean[@]}" >> "$outfile"
		# 🧹 Remove echoed command and exit from output
		escaped_line=$(printf '%s\n' "$line" | sed 's/[\/&]/\\&/g')
		sed -i "/^$escaped_line$/d" "$outfile"
		sed -i '/^exit$/d' "$outfile"
	else
		sed_clean=(
			-e '/^\w+@.*\$ .*/d'
			-e '/^exit$/d'
			-e 's/^(minishell|bash): //'
			-e 's/\x1B\[[0-9;]*[A-Za-z]//g'
			-e 's/^SHLVL=.*/SHLVL=42/'
			-e 's/^declare -x SHLVL=.*/declare -x SHLVL=42/'
			-e '/^_=.*/d'
			-e '/^declare -x _=.*/d'
			-e '/^OLDPWD=.*/d'
			-e '/^declare -x OLDPWD=.*/d'
			-e '/^_P9K_.*=.*/d'
			-e '/^declare -x _P9K_.*=.*/d'
		)
		echo "$line" | "$MINISHELL" 2>&1 | sed -E "${sed_clean[@]}" | sort >> "$outfile"
	fi

	echo "__CMD_END__" >> "$outfile"
	i=$((i+1))
done
