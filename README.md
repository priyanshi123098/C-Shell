# Custom Unix Shell 

This project is a custom Unix-like shell implemented in C. The shell replicates several Bash functionalities and introduces custom commands to enhance the user experience.

---

## Features

### Part A: Basic System Calls

#### 1. Display Prompt
- Format: `<Username@SystemName:Path>`
- The initial shell directory is considered the home directory (`~`)
- Prompt updates as the working directory changes
- Absolute paths are shown when outside the home directory

#### 2. Input Handling
- Supports multiple commands using `;` (sequential execution) and `&` (background execution)
- Handles random spaces and tabs
- Background processes print their process ID when started

#### 3. `hop` Command
- Custom implementation of `cd`
- Supports flags:
  - `~`: home directory
  - `.`: current directory
  - `..`: parent directory
  - `-`: previous directory
- Handles multiple arguments sequentially
- Displays the final absolute path after hopping

#### 4. `reveal` Command
- Mimics `ls` with custom implementation
- Supports:
  - `-a`: show all files including hidden ones
  - `-l`: long listing with file details
- Handles combinations like `-la`, `-al`, `-lala`
- Color codes:
  - Executables: green
  - Directories: blue
  - Regular files: white

#### 5. `log` Command
- Maintains history of last 15 unique commands (excluding consecutive duplicates)
- Commands with `;` or `&` are stored as one entry
- Subcommands:
  - `log`: display command history
  - `log purge`: clear the log
  - `log execute <index>`: execute a command from history

#### 6. System Command Execution
- Executes system programs like `vim`, `gedit`, etc.
- Foreground processes: shell waits until completion, and if >2s, prints duration
- Background processes: shell continues taking input, prints completion messages asynchronously

#### 7. `proclore` Command
- Displays information for a process (defaults to current shell if no PID provided)
  - PID
  - Status (R/R+/S/S+/Z)
  - Process group ID
  - Virtual memory usage
  - Executable path

#### 8. `seek` Command
- Recursively searches for files/directories by name or prefix
- Flags:
  - `-d`: directories only
  - `-f`: files only
  - `-e`: open/display if a unique match is found
- Relative paths printed from the target directory
- Handles invalid flags and permission errors gracefully

---

### Part B: Processes, Files, and Misc

#### 9. I/O Redirection
- Supports:
  - `>`: overwrite output file
  - `>>`: append to output file
  - `<`: read input from file
- Files are created if they don't exist
- Handles permission and file-not-found errors

#### 10. Pipe Support
- Supports chaining commands using `|`
- Executes from left to right
- Validates pipe placement (errors if nothing on either side)

#### 11. Redirection with Pipes
- Supports combining I/O redirection and pipes
- Example: `cat < input.txt | wc | cat > output.txt`

#### 12. `activities` Command
- Lists all background processes spawned by the shell
- Format:
  - `[PID] : [command name] - [State]`
  - State can be Running or Stopped
- Sorted in lexicographic order

#### 13. `ping` Command
- Sends a signal to a process: `ping <pid> <signal_number>`
- Takes modulo 32 of the signal number
- Prints confirmation or error if PID does not exist

- Also handles:
  - `Ctrl+C`: sends SIGINT to foreground process
  - `Ctrl+D`: exits shell, terminates all spawned processes
  - `Ctrl+Z`: moves foreground process to background and marks as stopped

#### 14. `fg` and `bg` Commands
- `fg <pid>`: brings background process to foreground
- `bg <pid>`: resumes a stopped background process

#### 15. `neonate` 
- `neonate -n <time_interval>`: prints PID of most recently created process every n seconds
- Stops when user presses `x`

---

### Part C: Networking

#### 16. `iMan` Command
- Fetches man pages using HTTP GET from `http://man.he.net`
- Usage: `iMan <command_name>`
- Prints raw HTML content of man page
- Ignores extra arguments

---

## Compilation & Running

```bash
make
./mysh
