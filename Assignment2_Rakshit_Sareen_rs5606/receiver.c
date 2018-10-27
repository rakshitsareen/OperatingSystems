#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
#define MAXSIZE     27
 
void die(char *s)
{
    perror(s);
    exit(1);
}

void freeptr (char **ptr){
    if (*ptr != NULL){
        free (*ptr);
        *ptr = NULL;
    }
}
 
int main()
{
    char c;
    int sharedmemoryid;
    key_t key;
    char *shm;
    key = 5678;
 
	while(1){

		char *buffer;
		size_t bufsize = 32;

		buffer = (char *)malloc(bufsize * sizeof(char));
		if( buffer == NULL)
			die("buffer");

		printf("\nEnter your secret line: ");
		getline(&buffer,&bufsize,stdin);
    
		char *word = "C00L";
		if(strstr(buffer, word) != NULL)
		{
    
			if ((sharedmemoryid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0) //create new memory segment using shmget
				die("shmget");
    
			if ((shm = shmat(sharedmemoryid, NULL, 0)) == (char *) -1) //Using shmat function to make shared memory available and return shared memory address
				die("shmat");
    
			printf("\nShared memory attached at: %p\n", shm);
    
			sprintf(shm, "%s", buffer); //printing buffer input to shared memory location
    
			system("./processor");
 
			shmdt(shm);	//dettaching the shared memory segment
    
			freeptr(&buffer);
		}
		
		else
			printf("\nEnter again with Secret Code");

	}    
 
    exit(0);
}

