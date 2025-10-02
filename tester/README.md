# Tester

> Automated parity harness for Minishell vs Bash.

## Quick Start
- `make` — run full suite, diff results, and emit `diff_summary.txt`.
- `make run` — regenerate raw outputs only (skip diff/summary).
- `make valgrind` — replay every case under Valgrind with `readline` suppressions.

```sh
make            # clean previous artefacts and execute the default testing flow
make valgrind   # run all commands with valgrind to check for leaks
make diff       # compare stored minishell vs bash outputs
make summary    # rebuild diff_summary.txt from the latest run
```

## Layout
- `test_cases_auto.txt` — commands executed in order; blank lines and `#` are ignored.
- `test_cases_manual.txt` — scenarios to validate manually before graduation to the auto list.
- `results/` — organised into `minishell/`, `bash/`, `diff/`, and `valgrind_logs/` for forensics.
- `scripts/` — reusable runners that normalise prompts, colours, SHLVL, and command markers (`__CMD_START__`/`__CMD_END__`).

## Extending Coverage
- Append a command to `test_cases_auto.txt`; rerun `make` to generate new numbered outputs.
- Use fixtures from `test_files/` (e.g., `invalid_permission`, `big_file`) to probe error messages and performance.
- Promote manual cases once behaviour is confirmed; the numbering logic adapts automatically.

## Reporting
- Green lines in the console indicate parity; red lines display failing commands.
- `diff_summary.txt` collates command, minishell output, bash output, and unified diff in one place.
- Valgrind verdicts aggregate in `results/valgrind_results.txt`; drill into the matching log inside `results/valgrind_logs/` for details.

## Notes
- `RUN_*` scripts expect the Minishell binary at the repository root (`../minishell`).
- `run_minishell.sh` and `run_bash.sh` sanitise escape codes and SHLVL so diffs focus on behaviour.
- Tidying up: `make clean` purges result directories, while `make fclean` removes the entire cache and summary artefacts.
