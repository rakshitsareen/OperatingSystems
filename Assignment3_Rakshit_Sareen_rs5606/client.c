#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<fcntl.h>

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int C, char *args[])
{

	int mysocket;
	char *check;
	struct sockaddr_in serveraddress;

	char *buffer = malloc(sizeof(char)*BUFSIZ);	//allocating memory for buffer dynamically
	
	if (C != 3){
        printf("INVALID ARGUMENTS, Enter the IP Address of the server to connect to\n");
		printf("The port configured for server is 4444%s\n", args[0]);
        exit(1);
    }
    
	//Create socket
	mysocket = socket(AF_INET, SOCK_STREAM, 0);
	if(mysocket < 0)
		die("ERROR");
		
    //Configuring the server to connect to
    memset(&serveraddress, 0, sizeof serveraddress );
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(atoi(args[2]));      // IP address is taken from the command line argument
    serveraddress.sin_addr.s_addr = inet_addr(args[1]); //Get address from arguments
    
	//connecting to the server
    if(connect(mysocket, (struct sockaddr*)&serveraddress, sizeof serveraddress ) < 0)
        die("Cannot Connect to server");
    
	else{
        printf("Connection successful\n");
        printf("To disconnect from the server and close the connection, Type EXIT\n");
	}
	
	while (1) {
 	
		// User is asked for an alpha numeric string input
		printf("Enter an alpha numeric along with the secret code: ");
		memset(buffer, 0, sizeof buffer );
		fgets(buffer, BUFSIZ, stdin);
        
        check = strstr (buffer,"EXIT");
        if (check!=NULL) {
            
            if (send(mysocket, buffer, strlen(buffer), 0) < 0)	//also sending the EXIT command to signal the server to close
                die("send error:");
            
            printf("The user has given exit command, closing server and client\n");
            exit(1);
            
        }
        
		check = strstr (buffer,"C00L");
        if (check!=NULL) {
			
            if (send(mysocket, buffer, strlen(buffer), 0) < 0)
                perror("send error:");
		}
        
	}
    
	close(mysocket);
    
    return 0;
}
