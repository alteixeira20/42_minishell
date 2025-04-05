import os

MINI_DIR = "results/minishell"
BASH_DIR = "results/bash"
DIFF_DIR = "results/diff"
SUMMARY_FILE = "diff_summary.txt"
TEST_CASES = "test_cases.txt"

def read_test_command(index):
    with open(TEST_CASES) as f:
        cmds = [line.strip() for line in f if not line.startswith('#') and line.strip()]
    if index - 1 < len(cmds):
        return cmds[index - 1]
    return "<command not found>"

def clean_output_lines(lines):
    return [line for line in lines if line.strip() not in ("__CMD_START__", "__CMD_END__")]

with open(SUMMARY_FILE, "w") as out:
    for diff in sorted(os.listdir(DIFF_DIR), key=lambda x: int(x.split("test")[1].split(".")[0])):
        test_num = int(diff.split("test")[1].split(".")[0])
        cmd = read_test_command(test_num)
        minishell_out = os.path.join(MINI_DIR, f"test{test_num}.out")
        bash_out = os.path.join(BASH_DIR, f"test{test_num}.out")

        out.write(f"===== Test {test_num} =====\n")
        out.write(f"Command: {cmd}\n\n")

        out.write("--- Minishell Output ---\n")
        with open(minishell_out) as f:
            out.writelines(clean_output_lines(f.readlines()))

        out.write("\n--- Bash Output ---\n")
        with open(bash_out) as f:
            out.writelines(clean_output_lines(f.readlines()))

        out.write("\n--- Diff ---\n")
        with open(os.path.join(DIFF_DIR, diff)) as f:
            out.write(f.read())

        out.write("\n" + "=" * 30 + "\n\n")
