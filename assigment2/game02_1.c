#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<semaphore.h>
#include<errno.h>
#include<fcntl.h> 
#include<sys/time.h>
#include<time.h>

#define SHM_SIZE 3072
typedef struct Data
{
    int a;
    int b;
    int winner;
    sem_t sem;
    long int acquire_lock_time;
    long int count;
}data_entry;

int main(int argc, char *argv[]){

    struct timeval parent_current_time;
    struct timeval child_current_time;    
    int P;
    int M;
    long int start_time;
    long int end_time;  
    
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
    data_entry *data;
    data = (data_entry*)shmat(shmid,(void*)0,0);
    if(data < 0 ){
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    //write 1 and 2 
    int max;
    data->a = 1;
    data->b = 2;
    //printf("%d writing to segment: %d,%d\n",getpid(),data->a,data->b);
    
    //Fork P child process
    pid_t id[P];
    //initailize a sem and variables using for calculate 
    sem_init(&(data->sem),1,1);
    data->acquire_lock_time = 0;
    data->count = 0;

    for(int i = 0; i < P; i++){
        id[i] = fork();
        if(id[i] < 0){
            perror("fork");
            exit(EXIT_FAILURE);
        }
         //child process
        else if(id[i] == 0){
            while(1){
                gettimeofday(&child_current_time,NULL);
                long int try_lock_time = child_current_time.tv_sec * 1000000 + child_current_time.tv_usec; 
                //printf("%d start to try lock at %ld\n",getpid(),try_lock_time);
                while(sem_trywait(&(data->sem)) < 0){
                    if(errno = EAGAIN)
                        continue;
                    else
                        perror("sem_trywait failed:");
                }
                gettimeofday(&child_current_time,NULL);
                long int get_lock_time = child_current_time.tv_sec * 1000000 +child_current_time.tv_usec;
                //printf("%d get lock at %ld\n",getpid(),get_lock_time);
                //calculate
                data->acquire_lock_time =  data->acquire_lock_time + get_lock_time - try_lock_time;
                data->count++;
                //printf("%d spend %ld to acquire a lock\n",getpid(),get_lock_time - try_lock_time); 
                if(data->a > M || data->b> M){
                    sem_post(&data->sem);
                    exit(0);
                }
                (data->a > data->b) ? (data->b = data->a + data->b) : (data->a = data->a + data->b);
                //printf("%d writing to segment: %d,%d\n",getpid(),data->a,data->b);
                data->winner = getpid();
                //printf("%d writing to segment: %d\n",getpid(),data->winner);
                sem_post(&(data->sem));
            }
        }    
        else{
            if(i==0){
                //get the start time of parent process
                gettimeofday(&parent_current_time,NULL);
                start_time = parent_current_time.tv_sec * 1000000 + parent_current_time.tv_usec;
            }
        }             
    }

    int ret;
    int status;
    //wait all child process to terminate
    for(int j = 0; j < P; j++){
        if(id[j]!=0){
            ret = waitpid(id[j],&status,0);
            //printf("parent %d with child %d ends at %ld\n",getpid(),id[j],end_time[j]);
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
    (data->a > data->b) ? (max = data->a) : (max = data->b); 
    //get the end time of parent process
    gettimeofday(&parent_current_time,NULL);
    end_time = parent_current_time.tv_sec * 1000000 + parent_current_time.tv_usec;
    // print the result 
    printf("winner value:%d\n",max);
    printf("winner pid:%d\n",data->winner);
    //calculate execution time of parent process and average time to acuire the lock for each child 
    printf("total exection time of the parent process is %f us \n",(double)(end_time - start_time));
    //printf("total time of acquiring lock is %ld\n",data->acquire_lock_time);
    //printf("total number of locks is %ld\n",data->count);
    printf("Average time to acquire the lock for each child is %f us \n",(double)(data->acquire_lock_time)/(double)(data->count));
    // detach from the segment:
    if (shmdt(data) == -1) {
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

    

   
    
   