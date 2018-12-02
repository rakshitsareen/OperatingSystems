#include <stdio.h>
#include <rpc/rpc.h>
#include "os.h"
#include <string.h>
main(int argc, char ** argv)
{
    CLIENT * client;
    int * result;
    char * server;
    char * message;
	char input[256];

    if (argc != 2) {
		fprintf(stderr, "Usage : %s <host> <message>\n",argv[0]);
		exit(1);
    }
    server = argv[1];

	while(1)
		{
		printf(" Enter the alpha numeric string with secret code: ");
		fgets(input,BUFSIZ,stdin);
		message=input;
	 
		client = clnt_create(server, ASSIGNMENT, ASSIGNMENTVERSION, "tcp");

		//error is shown if server doesn't connect 
		if (client == NULL) {
			clnt_pcreateerror(server);
			exit(1);
		}

		//calling the remote procedure
		result = myassignment_1(&message, client);
		
		//if server returns a NULL error msg is displayed
		if (result == (int *) NULL) {
			clnt_perror(client, server);
			exit(1);
		}
		
		//If no secret word C00L, below if message displayed , else the msg is stored in the secrets.out file
		if (*result == 0) {
			printf("Secret Code missing. Please retype\n");
		}
		
		else{
			printf("Message delivered to %s\n",server);
		}
	}
}


