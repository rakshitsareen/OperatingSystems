#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>

#define PORT 4444
#define BACKLOG 1

void die(char *s)
{
    perror(s);
    exit(1);
}

int lines=0, digits=0;

// Function to count the number of digits in the string
int total_digits(char *s)
{
    int i = 0;
    while (*s != '\0') {
    	if (isdigit(*s))
       		i++;
        s++;
    }

    return i;
}

//Handles the Ctrl-C interrupt and displays the summary
void my_handler(int s){
		printf("\nTotal number of lines received by the server is: %d \n",lines);
		printf("Total digit count across all input seen by the server is : %d \n ",digits);

}


int main(int C, char *V[])
{
    
    signal (SIGINT,my_handler);
    int mysocket, new_fd, buf_len, infinite_loop = 1,sum = 0;
	struct sockaddr_in serv_addr,client_addr;
	socklen_t sin_size;
	char s[100], buffer[BUFSIZ],*check;
	
	FILE * fptr;

	mysocket= socket(AF_INET,SOCK_STREAM,0);
	if(mysocket<0)
		die("Failed to create socket");

	// Initializing the server
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Error is raised if Bind Fails
    if (bind(mysocket,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) {
        close(mysocket);
        die("server: bind didn't work");
    }

	// Listens to the incoming sockets and waits for a connection, this is a blocking function
	if (listen(mysocket, BACKLOG) < 0) {
		die("listen: failed");
	}
    
	printf("server: waiting for connections\n");
	
	sin_size = sizeof(client_addr);
    new_fd = accept(mysocket, (struct sockaddr *)&client_addr, &sin_size);
    if (new_fd < 0) {
        perror("accept");
    }

    printf("server: got connection from %s \n", inet_ntop(AF_INET, &client_addr.sin_addr, s, sizeof (s)));

	while (1) {
        
		// error is raised if buffer is not received
		if ((buf_len = recv(new_fd, buffer, BUFSIZ-1, 0)) < 0)
			die("recv: couldn't receive from client");
			
		// '\0' to signalize the termination of the string.
		buffer[buf_len] = '\0';
			
        check = strstr (buffer,"EXIT");
        if (check!=NULL) {
            die("The user has given exit command, closing the server");
        }

		check = strstr (buffer,"C00L");
			if (check!=NULL) {
                lines++;
                
                // display string received
                printf("server: string received %s \n",buffer);
		
                fptr = fopen("secrets.out","a");
                sum = total_digits(buffer);  //calls the function to count the digits in the string
                digits = digits + sum;      // sums the digits seen across the server
                
                fprintf(fptr, "\nThe number of digits is: %d", sum);
                fprintf(fptr, "\nThe original line is: %s", buffer);
                fclose(fptr);
			}	
			
		close(mysocket);                // closing the socket.
	}

    return 0;
	
}
