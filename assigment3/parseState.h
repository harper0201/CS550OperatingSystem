#ifndef PARSE_FILE_H
#define PARSE_FILE_H

//function to parse file states
char * parseState(long state){
   char* result;
    switch(state){
        case 0 :
            result = "TASK_RUNNING";
            break;
        case 1 :
            result = "TASK_INTERRUPTIBLE";
            break;
        case 2 :
            result = "TASK_UNINTERRUPTIBLE";
            break;
        case 4 :
            result = "__TASK_STOPPED";
            break;
        case 8 :
            result = "__TASK_TRACED";
            break;
        case 16:
            result = "EXIT_DEAD";
            break;
        case 32 :
            result = "EXIT_ZOMBIE";
            break;
        case 32 | 16 :
            result = "EXIT_TRACE";
            break;
        case 64:
            result = "TASK_PARKED";
            break;
        case 128:
            result = "TASK_DEAD";
            break;
        case 256:
            result = "TASK_WAKEKILL";
            break;
        case 512:
            result = "WAKING";
            break;
        case 1024:
            result = "TASK_NOLOAD";
            break;
        case 2048:
            result = "TASK_NEW";
            break;
        case 4096:
            result = "TASK_ RTLOCK_WAIT";
            break;
        case 8192:
            result = "TASK_ STATE_MAX";
            break;
        case 256 | 2:
            result = "TASK_KILLABLE";
            break;
        case 256 | 4:
            result = "TASK_STOPPED";
            break;
        case 256 | 8:
            result = "TASK_TRACED";
            break;
        case  2 | 1024:
            result = "TASK_IDLE";
            break;
        case 1 | 2:
            result = "TASK_NORMAL";
            break;
        case 0 | 1 | 2 | 4 | 8 | 16 | 32 | 64 :
            result = "TASK_REPORT";
            break;
        default : 
           break; 
    }
       return result;
}

#endif