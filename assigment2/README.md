# Assigment2：Multi-Process Addition Game Without Locking
## Installation
xhong4@CS550-xhong4
Linux
## Usage
Make
```sh
make
```
M = 50000, P = 10,20,30,40,50,60,70,80,90,100
```sh
make 1 
make 2
make 3
make 4
make 5
make 6
make 7
make 8
make 9 
make 10
```
clean 
```sh
make clean
```
## Description of task
## _game01.c_
The main (parent) process does the following:
- Creates a shared memory region that hold at least three variables -- two numbers and a winner PID -- plus any additional information you want to keep.
- Writes two positive numbers, 1 and 2, into the two numbers in the shared memory region.
- Forks P child processes.
- Waits for all the children to complete.
- Prints of the larger of the two numbers and the winner PID from the shared memory.
- Exits.

Each child process repeats the following steps in a loop until the larger of the two numbers in the shared memory is greater than M (passed via command-line arguments). Don't introduce any locking yet! Wait to do that in Part B below.
- If any one of the two numbers in the shared memory is greater than M, then exit.
- Else replace the smaller number in the shared memory with the sum of the two numbers.
- If the result of addition is greater than M, then write this child's process ID into a "winner" variable in the shared memory and exit.

## _game02_1.c_
Busy-wait locking with "sem_trywait()" and "continue" 
## _game02_2.c_
Busy-wait locking with "sem_trywait()" and "sched_yield()" 
## _game02_3.c_
Busy-wait locking with "sem_wait()" 




