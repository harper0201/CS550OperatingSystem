#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>

#define SHM_SIZE 3072
int main(int argc, char *argv[]){
    int P;
    int M;
    if(argc == 3){
        P = atoi(argv[1]);
        M = atoi(argv[2]);
    }
    else{
        printf("please input two valid parameters");
        return 1;
    }

    //error handle 
    if(P < 0 || M < 0 ){
        printf("invalid input");
    }

    //create a shared memory
    int shmid;
    if((shmid = shmget(IPC_PRIVATE,SHM_SIZE,0644|IPC_CREAT|IPC_EXCL))<0){
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    //attach to the segment to get three pointers to it
    //three pointer donotes first number, secound number, winner pid
    int *a;
    int *b;
    int *winner;

    a = (int*)shmat(shmid,(void*)0,0);
    if(a == (int*)(-1)){
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    b = a + 1;
    winner = b + 1;
    
    //write 1 and 2 
    int max;
    *a = 1;
    *b = 2;
    //printf("writing to segment: %d,%d\n",*a,*b);

    //Fork P child process
     pid_t id[P];
    for(int i = 0; i < P; i++){
        id[i] = fork();
        if(id[i] < 0){
            perror("fork");
            exit(EXIT_FAILURE);
        }
         //child process
        else if(id[i] == 0){
            while(1){
                if(*a > M ||*b > M){ 
                    exit(0);
                }
                (*a > *b) ? (*b = *a + *b) : (*a = *a + *b);
                //printf("writing to segment: %d,%d\n",*a,*b);
                *winner = getpid();
                //printf("writing to segment: %d\n",*winner);
            }
            exit(0);
        }
    }

    
    int ret;
    int status;
    //wait all child process to terminate
    for(int j = 0; j < P; j++){
        if(id[j]!=0){
            ret = waitpid(id[j],&status,0);
            if(ret < 0){
                perror("wait");
                exit(EXIT_FAILURE);
            }
            else{
                //printf("%d:Child exited with status %d\n", id[j],WEXITSTATUS(status));
            }
        }
    }
    // get the larger one in two numbers
    (*a > *b) ? (max = *a) : (max = *b); 
    // print the result 
    printf("winner value:%d\n",max);
    printf("winner pid:%d\n",*winner);

    // detach from the segment:
    if (shmdt(a) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
    // delete the segment:
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
    return(0);
}
