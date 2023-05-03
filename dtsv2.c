#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "BXP/bxp.h"
#include <unistd.h>
#include <signal.h>
#include <assert.h>

//This work is my own. The only thing that I didnt consider doing was using a switch case for my workerthread to d
//differintate between different commands

#define UNUSED __attribute__((unused))
#define PORT 19999
#define SIZE 10000


int  queryRes(char *buf, char *sep, char *words[]) {
    int i;
    char *p;
    char tmp[SIZE];
    strncpy(tmp, buf, SIZE);

    for (p = strtok(tmp, sep), i = 0; p != NULL; p = strtok(NULL, sep), i++)
        words[i] = p;
    words[i] = NULL;
    printf("value of i: %d\n", i);
    //char command;
    //use strcmp to compare the Command and the string of allowed commands 
    if(!strcmp(words[0], "OneShot") && i == 7){
    	//command = words[0];
    	return 1; 
    }else if(!strcmp(words[0], "Repeat") && i == 9){
    	//command = words[0];
    	return 2; 
    }else if(!strcmp(words[0], "Cancel") && i == 2){
    	//command = words[0];
    	return 3; 
    }else{
    	return 0; 
    	//return command = 'Invalid';
    }
    //if(words[0] == 'Repeat' && i == 8){
    return 0;
}




void *threadfunc(UNUSED void *args){
	printf("thread created\n");
	int commandInt;
	int validBit = 0; 
	BXPService svc;
	BXPEndpoint ep; 
	char query[SIZE], response[SIZE + 1];
	unsigned qlen, rlen;
	assert(bxp_init(PORT, 1));
    assert((svc = bxp_offer("DTS")));
	//use a switch statement for cleaner use 
	while((qlen = bxp_query(svc, &ep, query, 10000)) >0){
		char *words[25];
		commandInt = queryRes(query, "|", words);
		switch(commandInt){
				case 0: //printf("ERROR: Invalid Command\n");
					validBit = 0; 
					break;
				case 1: validBit = 1; 
					break;
				case 2: validBit = 1; 
					break;
				case 3: validBit = 1;
					break;
		}

		sprintf(response, "%d%s", validBit, query);
       	rlen = strlen(response) + 1;
       	//printf("%c\n", response[0]);
		assert(bxp_response(svc, &ep, response, rlen));

	}

	return NULL;
}


int main(UNUSED int argc, UNUSED char *argv[]){
	pthread_t WorkerThread;
    pthread_create(&WorkerThread, NULL, &threadfunc, NULL);
    pthread_join(WorkerThread, NULL);
}

