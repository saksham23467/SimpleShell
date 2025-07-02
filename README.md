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

```txt
shell.c       --> Main shell source code  
README.md     --> This file
```
