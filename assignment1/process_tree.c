#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

int main(int argc,char* argv[]){

    // Read two arguments from command line.
    // L denotes the level of tree, N denotes the number of children of each internal node.
    int L,N;
    if(argc ==3){        
        
        L = atoi(argv[1]);
        N = atoi(argv[2]);
    }
    else{
        printf("please input two parameter L and N");
        return 1;
    }

    // L and N should be valid input
    if(L <=0 || N <=0){
        printf("Invalid input.L and N should be positive interger.");
        return 1;
    }

    //Output information about process tree (proccess id,parent id ,level of tree)
    int pid = getpid();
    int ppid = getppid();
    printf("%d:process starting\n",pid);
    printf("%d:parent's id = %d\n",pid,ppid);
    printf("%d:Level in the tree = %d\n",pid,L);
  
    //if level of tree is 1, process tree is completed.
    //if level of tree is greater than 1, create N child process.
    if(L == 1){
        exit(0);
    }
    else{
        printf("%d:Creating %d children at level %d\n",pid,N,L-1);
    }

    pid_t id[N];
    pid_t root;
    int ret;
    int status;
    for(int i = 0; i < N; i++){
        id[i] = fork();

        if(id[i]<0){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        //child process
        else if(id[i]==0){
            char n[100];
            char l[100];
            //printf("parent:%d,child:%d,level:%d\n",getppid(),getpid(),L-1);
            //transfer int to string
            int transfer_n_flag = sprintf(n, "%d", N);
            int transfer_l_flag = sprintf(l,"%d",L-1);
            if (transfer_n_flag < 0 || transfer_l_flag < 0){
                perror("transfer failed");
                exit(EXIT_FAILURE);
            }
            //Each child do recursion
            char *pathname = "./process_tree";
            int exec_return_value = execlp(pathname,pathname,l,n,(char*)NULL);  
            if(exec_return_value == -1){
                perror("execlp");
                exit(EXIT_FAILURE);
            }         
        }
        //parent process
        else{           
            //printf("Parent:%d,Child:%d,LEVEL:%d\n",getpid(),id[i],L);
        }
    }
    
    // wait for all children process terminate 
    for(int j = 0 ; j < N; j++){
        if(id[j] != 0){
            ret = waitpid(id[j],&status,0);
            if(ret<0){
                perror("wait");
                exit(EXIT_FAILURE);
            }
            else{
                printf("children %d exited with status %d\n",id[j],WEXITSTATUS(status));
            }
        }
    }
    root = getpid();
    printf("%d:Terminating at level:%d\n",root,L);
}

