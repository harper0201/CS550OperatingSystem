#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H

#define MAX_BUF_SIZE 100000

struct my_buf {
    unsigned long pid;
    unsigned long ppid;
    unsigned long state;
    unsigned long cpu;

};

#endif //PROCESS_LIST_H
