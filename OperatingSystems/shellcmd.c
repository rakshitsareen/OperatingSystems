//
//  shellcmd.c
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

/*
 Run the command. Takes path, command and the argument.
 */
int execute(char* path, char* cmd, char* args){
    int return_val;
    //sanitizing the command
    cmd[strcspn(cmd, "\n")] = 0;    // removing trailing \n. because we used fgets. *THIS IS IMPARTANT IF WE ARE USING fgets()*
    strcat(path,cmd);                   // concatenate the command to path
    if(args == NULL){
        return_val = execl(path,cmd,NULL,NULL);  // execute the command
    }
    else {
        // sanitize the argument
        args[strcspn(args, "\n")] = 0;  // removing trailing \n. because we used fgets. *THIS IS IMPARTANT IF WE ARE USING fgets()*
        return_val = execl(path, cmd, args, NULL);
        if(return_val==-1){  // if the previous command did not run, then try /bin path
            strcpy(path,"/bin/");   // concatenate '/bin' to the path
            strcat(path,cmd);   // concatenate comand to path
            return_val = execl(path,cmd,args,NULL);  // execute the command
            if(return_val == -1){  // not able to run at all.
                printf("cannot execute command\n");
            }
        }
    }
    return return_val;
}

int main()
{
    char cmd[50], args[50], cmd_path[50];
    char args_or_not;
    int PID, status;
    while(1) {        //infinite loop to receive input from user, use ctrl + c to kill the program
        printf("Please enter the command to execute : ");
        if(fgets(cmd, 50, stdin) != NULL){
            printf("Enter 'y' for arguments or 'n' to not ");
            scanf("%c", &args_or_not);
        }
        else {
            args_or_not = 'n';
        }
        getchar();
        
        if((PID = fork()) == 0) { //child process gets 0 on fork, so this gets executed only by the child process
            if(args_or_not=='y') {      // if the user entered the yes for arguments
                printf("Enter arguments (with hyphen : -) ");
                if(fgets(args, 50, stdin) != NULL){
                    //getchar();
                    strcpy(cmd_path,"/usr/bin/");
                    execute(cmd_path, cmd, args);
                }
            }
            else {  // if the user did not enter yes for the arguments
                strcpy(cmd_path,"/bin/");
                execute(cmd_path, cmd, NULL);
            }
            exit(0);
        }
        else if(PID<0){ //fork error if processid negative
            status= -1;
        }
        else {
            if(waitpid(PID, &status,0)!= PID) // parent process gets processid on fork
                status= -1;
        }
    }
    return 0;
}
