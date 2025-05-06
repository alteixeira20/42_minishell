#!/bin/bash

MINISHELL=$1
TEST_FILE=$2
TMP_ENV=$3
OUT_DIR=$(dirname "$TMP_ENV")

cd "$(cd "$(dirname "$0")/.." && pwd)" || exit 1
mkdir -p "$OUT_DIR"

i=1

# Define common sed script for cleaning output
common_sed_script=$(
cat <<'EOF'
/^[^ ]+@[^:]+:[^$]+\$>? ? /d
/^\/.*\$ .*/d
/^exit$/d
s/^(minishell|bash|msh): //g
s/^(minishell|bash|msh)> //g
s/^(minishell|bash|msh)$ //g
s/^(minishell|bash|msh)$> //g
s/^(minishell|bash|msh) $ //g
/^.*msh\$ .*/d
s/\x1B\[[0-9;]*[A-Za-z]//g
/^declare -x _=.*$/d
/^_=.*/d
/^-> .*/d
s/^declare -x SHLVL=.*/declare -x SHLVL=42/
s/^SHLVL=.*/SHLVL=42/
s/^minishell> //g
EOF
)

# Process each non-comment, non-empty line in the test file
grep -vE '^\s*#|^\s*$' "$TEST_FILE" | while IFS= read -r line; do
    outfile="${OUT_DIR}/test$i.out"
    echo "__CMD_START__" > "$outfile"

    if [[ "$line" != "env" ]]; then
        "$MINISHELL" 2>&1 <<EOF | sed -E "$common_sed_script" >> "$outfile"
$line
exit
EOF
        # Remove echoed command and exit from output
        escaped_line=$(printf '%s\n' "$line" | sed 's/[\/&]/\\&/g')
        sed -i "/^$escaped_line$/d" "$outfile"
        sed -i '/^exit$/d' "$outfile"
    else
        "$MINISHELL" 2>&1 <<EOF | sed -E "$common_sed_script" | sort >> "$outfile"
$line
exit
EOF
    fi

    echo "__CMD_END__" >> "$outfile"
    i=$((i + 1))
done
