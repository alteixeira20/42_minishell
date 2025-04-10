# 🐚 MiniShell

> A minimalist Bash-like shell written in C as part of the 42 Common Core curriculum.

---

## Overview

**Minishell** is a simple command-line interpreter built to replicate core functionalities of a POSIX-compliant shell. It handles command parsing, redirections, pipes, environment variables, and signal management — all within the strict constraints of the 42 Norm.

The project offers a deep dive into:

- Process control & execution
- File descriptors & redirection
- Signal handling (e.g., `Ctrl-C`, `Ctrl-D`)
- Environment variable expansion
- Builtin command integration

---

## 🚀 Getting Started

```bash
git clone https://github.com/alteixeira20/42_minishell.git
cd 42_minishell
make
./minishell
```

---

## 📚 Documentation

Comprehensive documentation is available in our **GitHub Wiki**, covering:

- Architecture & Design Strategy  
- Code Structure & Module Breakdown  
- Norm Compliance & Best Practices  
- Custom Testing & Debugging Tools  
- Makefile & Build System Explained  

👉 **Start here**: [MiniShell Wiki](../../wiki)

---

## ✅ Builtin Commands

- `echo`
- `cd`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

---

## ⚠️ Constraints

- Only **one global variable** is allowed
- No memory leaks (except known `readline()` suppression)
- Must be compliant with `bash` behavior for mandatory cases

---

## 📜 License

This project is part of the **42 School** educational program and is subject to its internal evaluation criteria.

---

> ✨ For implementation details, head over to the [Wiki](../../wiki).
