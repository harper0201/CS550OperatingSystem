#include "processList.h"
#include "parseState.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <unistd.h> 
           

struct my_buf mybuf[MAX_BUF_SIZE];

  int main () { 
	                                    
    int fd;                                       
    int bytes_read;                                        
    int count = 0;
    int i;
   
   //Open device
   //#include <fcntl.h>
   //int open(const char *pathname, int flags); 
   //flags:O_RDONLY, O_WRONLY, or O_RDWR                          
   fd = open("/dev/process_list", O_RDONLY);

    if ( fd < 0 ) {                                 
      printf("open error.\n");  
      exit(1);                                        
    } 

  //read device
  //#include <unistd.h>
  //ssize_t read(int fd, void *buf, size_t count);
  bytes_read =read(fd, mybuf, sizeof(mybuf)); 
    if ( bytes_read < 0 ) {   
      printf("read error.\n");
      exit(1);                              
    }   
    while(count < bytes_read ){
       printf( "PID= %ld  PPID= %ld  CPU= %ld   STATE= %s\n",mybuf[count].pid , mybuf[count].ppid ,mybuf[count].cpu ,parseState(mybuf[count].state));  
       count++;
    }
    close(fd);                                      
    return 0;                                       
  }
