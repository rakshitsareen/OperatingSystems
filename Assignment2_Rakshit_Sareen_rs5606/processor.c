#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXSIZE     27
 
void die(char *s)
{
    perror(s);
    exit(1);
}

int main()
{
    int sharedmemoryid;
    key_t key;
    char *shm;
    FILE *fp;
    char *storage;
    key = 5678;
    int count = 0;
 
    if ((sharedmemoryid = shmget(key, MAXSIZE, 0666)) < 0) //accessing the same shared memory using key
        die("shmget");
 
    if ((shm = shmat(sharedmemoryid, NULL, 0)) == (char *) -1) //retrieving the address of shared memory
        die("shmat");
      
    storage = shmat(sharedmemoryid, NULL, SHM_RDONLY);
    printf("\nThis line was stored in secrets file: %s", storage);
    fp = fopen("secrets.out","a"); 
 
    for(int i = 0; i<strlen(storage); i++){
        if(isdigit(storage[i])){
            count++;
        }
    }
	
	//printing digit count and original line to file secrets.out
    fprintf(fp, "\nThe number of digits is: %d", count);
    fprintf(fp, "\nThe original line is: %s", storage);
    fclose(fp);
   
    shmdt(shm); //dettaching the shared memory segment

    exit(0);
}
