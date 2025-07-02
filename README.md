# 🐚 Simple C Shell

A simple Unix-like shell implemented in C that supports:
- Command execution (foreground and background)
- Command history tracking
- Signal handling (e.g., Ctrl+C)
- Piped command execution (`|`)
- ASCII art welcome message
- Colored terminal output

---

## 🚀 Features

### ✅ Core Functionalities
- **Execute commands** like `ls`, `echo`, `pwd`, etc.
- **Pipes (`|`)**: Supports up to 10 piped commands (e.g., `ls | grep txt | wc`)
- **Background execution** using `&` (e.g., `sleep 5 &`)
- **Command history**: Maintains last 200 commands with:
  - PID
  - Execution time
  - Foreground/background status

### 🎨 UI Features
- ASCII banner printed on shell start
- Colored prompt with username and path info

### 🛑 Signal Handling
- Handles `SIGINT` (Ctrl+C) gracefully
- Prints command history before exiting

---

## 📁 File Structure

shell.c --> Main shell source code
README.md --> This file



---

## 🧑‍💻 How to Compile

```bash
gcc shell.c -o my_shell
🧪 How to Run
bash

./my_shell
Example usage:


ls -l
history
echo Hello & sleep 2 &
ls | grep c | wc -l
exit
📝 Commands Supported
Any command available in your system’s $PATH, e.g.:

ls, pwd, cat, grep, echo

Background: sleep 10 &

Piping: ls | grep .c | wc -l

Special:

history: Show command history

exit: Exit shell and print history

🧠 Implementation Details
Uses fork() and execvp() to spawn child processes

Pipes are created using pipe(), and handled with dup2() and close()

Maintains an array of CommandHistory structs to track metadata

ANSI escape codes used for colored output

⚠️ Limitations
Supports up to 200 history entries (overwrites oldest when full)

Piping supports up to 10 commands

No support for I/O redirection (>, <) or complex shell features like globbing or variables

Limited to basic command parsing (no quotes, escaping)

👨‍💻 Authors
2023467

2023325

 📸 Example Prompt

2023467_2023325@Hello_World:~$
📞 Signal Handling Demo
Pressing Ctrl+C will:

Print history

Exit the shell cleanly

📚 License
This project is open source and free to use for educational purposes.
