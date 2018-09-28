//
//  main.c
//  OperatingSystems
//
//  Created by Rakshit Sareen on 9/14/18.
//  Copyright © 2018 Rakshit Sareen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int run_command(char* path, char* command, char* argument){
    int d;
    strcat(path,command);
    
    if(argument == NULL){
        d = execl(path,command,NULL,NULL);
    }
    else{
        d = execl(path, command, argument, NULL);
        if(d==-1){
            strcpy(path,"/bin/");
            strcat(path,command);
            d=execl(path,command,argument,NULL);
        }
    }
    return d;
}

int main(int argc, const char * argv[]) {
    char command[50], argument[50], path[50];
    char ch;
    int processid, status;
    for(;;) //infinite loop to receive input from user
    {
        printf(" Enter Command to Execute ");
        gets(command);
        //getchar();
        printf(" Enter 'y' to input arguments or 'n' to not ");
        scanf("%c", &ch);
        getchar();
        
        if((processid=fork()) == 0) //child process gets 0 on fork
        {
            if(ch=='y'){
                printf("Enter arguments (remember hyphen '-')");
                gets(argument);
                strcpy(path,"/usr/bin/");
                run_command(path, command, argument);
            }
            else{
                strcpy(path,"/bin/");
                run_command(path, command, NULL);
            }
        }
        
        else if(processid<0){
            //fork error if processid negative
            status= -1;
        }
        
        else{
            if(waitpid(processid, &status,0)!= processid)// parent process gets processid on fork
                status= -1;
        }
    }
    
    return 0;
}
