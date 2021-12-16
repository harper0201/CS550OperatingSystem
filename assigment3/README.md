## Assigment3: part E,F
Write a kernel module to return list of processes to user space via a character device. Implement a kernel module that creates a /dev/process_list character device. The character device should support the read() operation. When the read() system call is invoked on your character device from a user space process, your kernel module should return to the following information about all currently running processes:process ID, parent process ID,the CPU on which the process is running,its current state.Write a user space program to retrieve the list of processes from the kernel module. Implement a user-space C program that opens your character device and outputs the list of processes retrieved from your character device.

## Usage
```
make
make load
make access
make run
make unload
make clean
