#include <stdio.h>
#include "os.h"
#include <string.h>

int * myassignment_1_svc( char ** msg, struct svc_req * req)
{
    FILE *f;
    static int result;

    f = fopen("secrets.out","a");
    
    if (f == (FILE *) NULL) {
		result = 0;
		return(&result);
    }

    int l = strlen(*msg);
    int i = 0;
    char s[1000];
    strcpy(s,*msg);

    int flag = 0;
	 
    for(i=0;(i+3)<l;i++){
	    if(s[i]=='C'&&s[i+1]=='0'&&s[i+2]=='0'&&s[i+3]=='L'){
		   flag = 1;
		   break;
	    }
    }
	
    if (flag == 0){
	    fclose(f);
	    result = 0; //return if string does not have secret word
		printf("Missing secret word. Retype please\n");
	    return (&result);
    }
	
	printf("PASSWORD ACCEPTED!)\n");
    int count = 0;
    for(i=0;i<l;i++){
	    if(s[i]>='0'&&s[i]<='9'){
		    count ++;
	    }
    }
	
	count=count-2; //subtracting the zeros of C00L
    fprintf(f,"%d :%s\n",count,*msg);
    fclose(f);
    result = 1;

    return(&result);
	
}

