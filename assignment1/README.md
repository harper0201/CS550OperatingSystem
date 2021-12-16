# Assigment1: Process Tree

This is a program is to create a process tree with L-level whose internal node has N children, print information about processes as they are created and destroyed. 

## Installation

Install Linux to run.([Ubuntu](https://ubuntu.com/wsl)) 

Enable WSL
```sh
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
```

Install Ubuntu on Windows 10 and check its version
```sh
cat /etc/issue
```
## Usage
- Read two arguments from command line - the number of levels of the tree (L) and number of children of each internal node of the process tree (N). for example
```sh
$ ./process_tree 4 3
```

- Then the following information are printed:
		(pid): Process starting
		(pid): Parent's id = (ppid)
		(pid): Level in the tree = (value_of_L_from_command_line)
		(pid): Creating (value_of_N_from_command_line) children at Level (L-1)
- Next, if the Level L is greater than 1, N child processes are created using fork(), and wait for all of the children to complete using the wait() system call. Each child process do recursion while the parent waits.
- Once all the child processes (if any) have terminated, your program should quit by printing : (pid): Terminating at Level (L).