#!/bin/bash

MINISHELL_EXEC="$1"
TEST_FILE="$2"
MINI_TMP="$3"
MINI_OUT="$4"
BASH_TMP="$5"
BASH_OUT="$6"
MODE="${7:-stdout}"  # default to 'stdout' mode if not specified

SESSION="tester_session"

tmux kill-session -t $SESSION 2>/dev/null

tmux new-session -d -s $SESSION -c "$MINI_TMP" "$SHELL"
tmux split-window -h -t $SESSION -c "$BASH_TMP" "$SHELL"

# Launch shells
tmux send-keys -t $SESSION.0 "$MINISHELL_EXEC" C-m
tmux send-keys -t $SESSION.1 "bash" C-m

sleep 0.3

i=1
while IFS= read -r cmd || [ -n "$cmd" ]; do
	if [[ "$cmd" =~ ^\#.* ]] || [[ -z "$cmd" ]]; then
		continue
	fi

	# Clear both panes
	tmux send-keys -t $SESSION.0 "clear" C-m
	tmux send-keys -t $SESSION.1 "clear" C-m

	# Clear scrollback **before** capturing new output
	tmux clear-history -t $SESSION.0
	tmux clear-history -t $SESSION.1

	# Minishell: Start + Command + End
	tmux send-keys -t $SESSION.0 'echo __CMD_START__' C-m
	tmux send-keys -t $SESSION.0 "$cmd" C-m
	tmux send-keys -t $SESSION.0 'echo __CMD_END__' C-m

	# Bash: Start + Command + End
	tmux send-keys -t $SESSION.1 'echo __CMD_START__' C-m
	tmux send-keys -t $SESSION.1 "$cmd" C-m
	tmux send-keys -t $SESSION.1 'echo __CMD_END__' C-m

	sleep 0.5  # Allow commands to finish

	# Capture panes
	tmux capture-pane -p -J -t $SESSION.0 > "$MINI_OUT/raw_test$i.out"
	tmux capture-pane -p -J -t $SESSION.1 > "$BASH_OUT/raw_test$i.out"

	if [ "$MODE" = "stdout" ]; then
		awk '/__CMD_START__/,/__CMD_END__/' "$MINI_OUT/raw_test$i.out" | grep -v -E '^[^[:space:]]+@[^:]+:|^[[:alnum:]/._-]+[$#] ' \
			> "$MINI_OUT/test$i.out"
		awk '/__CMD_START__/,/__CMD_END__/' "$BASH_OUT/raw_test$i.out" | grep -v -E '^[^[:space:]]+@[^:]+:|^[[:alnum:]/._-]+[$#] ' \
			> "$BASH_OUT/test$i.out"
	fi

	rm -f "$MINI_OUT/raw_test$i.out" "$BASH_OUT/raw_test$i.out"

	i=$((i+1))
done < "$TEST_FILE"

# Exit both shells
tmux send-keys -t $SESSION.0 "exit" C-m
tmux send-keys -t $SESSION.1 "exit" C-m

sleep 0.2
tmux kill-session -t $SESSION 2>/dev/null
