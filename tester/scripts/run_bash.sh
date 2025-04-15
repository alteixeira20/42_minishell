#!/bin/bash

TEST_FILE=$1
TMP_ENV=$2
OUT_DIR=$(dirname "$TMP_ENV")

cd "$(cd "$(dirname "$0")/.." && pwd)" || exit 1

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
			-e 's/^line [0-9]+: ?//'
			-e 's/^-c: line [0-9]+: ?//'
			-e 's/\x1B\[[0-9;]*[A-Za-z]//g'
			-e "/^\`.*'$/d"
			-e 's/^declare -x SHLVL=.*/declare -x SHLVL=42/'
			-e '/^declare -x _=.*$/d'
		)
bash 2>&1 <<EOF | sed -E "${sed_clean[@]}" >> "$outfile"
$line
exit
EOF
	else
		sed_clean=(
			-e '/^\w+@.*\$ .*/d'
			-e '/^exit$/d'
			-e 's/^(minishell|bash): //'
			-e 's/^line [0-9]+: ?//'
			-e 's/^-c: line [0-9]+: ?//'
			-e 's/\x1B\[[0-9;]*[A-Za-z]//g'
			-e "/^\`.*'$/d"
			-e 's/^SHLVL=.*/SHLVL=42/'
			-e 's/^declare -x SHLVL=.*/declare -x SHLVL=42/'
			-e '/^_=.*/d'
			-e '/^declare -x _=.*/d'
		)
bash 2>&1 <<EOF | sed -E "${sed_clean[@]}" | sort >> "$outfile"
$line
exit
EOF
	fi

	echo "__CMD_END__" >> "$outfile"
	i=$((i+1))
done
