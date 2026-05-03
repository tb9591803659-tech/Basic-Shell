# 🐚 Basic Shell in C (Custom Unix-Like Shell)

A lightweight Unix-style shell implemented in C that supports command execution, pipelines, built-in commands, and a persistent command history system.

This project demonstrates core Operating System concepts such as process creation, inter-process communication, and memory management using system calls like `fork()`, `execvp()`, `pipe()`, and `waitpid()`.

---

## 📁 Project Structure

Basic-Shell/
├── Shell.c        → Main shell implementation (command execution, pipes, built-ins)
├── history.c      → Command history system implementation
├── history.h      → Header file for history module
├── History.txt    → Persistent command history file (auto-generated)

---

## 🚀 Features

### 🧠 Core Shell Features
- Custom shell prompt (`myshell>`)
- Executes system commands like `ls`, `pwd`, `echo`, etc.
- Process creation using `fork()`
- Command execution using `execvp()`
- Parent waits using `waitpid()`

---

### 🛠 Built-in Commands
- `cd <dir>` → Change directory
- `pwd` → Print current working directory
- `history` → Show command history
- `exit` → Exit shell safely

---

### 📜 Command History System
- Stores all executed commands dynamically
- Persistent storage in `History.txt`
- Supports:
  - `!!` → Repeat last command
  - `!n` → Execute nth command from history
- Implemented using dynamic memory (`malloc`, `realloc`, `strdup`)

---

### 🔗 Pipeline Support
Supports multi-command pipelines:

---

### 🧠 Concepts Used
- Process Management (`fork`, `execvp`, `waitpid`)
- Inter-Process Communication (`pipe`, `dup2`)
- File Handling (`fopen`, `fgets`, `fprintf`)
- Dynamic Memory Allocation (`malloc`, `realloc`, `strdup`, `free`)
- String Parsing and Tokenization (`strtok`, trimming, manual parsing)
- Command Execution in Unix-like systems
- Data Structures (Dynamic array implementation for command history)
- System Calls in Linux environment

---

### ⚠️ Known Limitations
- No support for input/output redirection (`>`, `<`, `>>`)
- No background process execution (`&`)
- Limited parsing (space-based tokenization only)
- Quotes handling (`"multi word arguments"`) not fully supported
- No signal handling (e.g., Ctrl+C customization)
- Pipeline implementation depends on `strtok`, which may be fragile in edge cases
- No command autocomplete or suggestion system
- Error handling can be improved for system call failures
- No support for environment variables (`export`, `$VAR`)

---

- ### 🔧 Future Improvements
- Add input/output redirection (`>`, `<`, `>>`)
- Add background process execution (`&`)
- Improve parser to support quotes and escape characters
- Add signal handling (`SIGINT`, `Ctrl+C`)
- Implement command autocomplete feature
- Improve modular design (separate parser, executor, and built-ins)
- Add environment variable support (`export`, `echo $VAR`)
- Enhance pipeline handling robustness
- Add job control system (`jobs`, `fg`, `bg`)
- Improve error handling for all system calls

---

- ### 📌 What This Project Demonstrates
- - Understanding of Unix/Linux shell internals
- Process creation and management using system calls
- Inter-process communication using pipes
- Low-level system programming in C
- Memory management and dynamic allocation techniques
- Command parsing and execution pipeline design
- Building a mini operating system shell from scratch

---

- ### 👨‍💻 Author
- Developed by a systems programming learner to deeply understand operating system concepts by implementing a Unix-like shell in C from scratch.

Focused on learning:
- Process handling
- System calls
- Memory management
- Command execution design
