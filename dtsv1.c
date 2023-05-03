#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "BXP/bxp.h"
#include <unistd.h>
#include <signal.h>
#include <assert.h>

#define UNUSED __attribute__((unused))
#define PORT 19999

void *threadfunc(UNUSED void *args){
	BXPService svc;
	BXPEndpoint ep;
	char query[10000], response[10001];
	unsigned qlen, rlen;
	assert(bxp_init(PORT, 1));
    assert((svc = bxp_offer("DTS")));
	while((qlen = bxp_query(svc, &ep, query, 10000)) >0){
		sprintf(response, "1%s", query);
       	rlen = strlen(response) + 1;
       	printf("%c\n", response[0]);
		assert(bxp_response(svc, &ep, response, rlen));
	}
	return NULL; 
}

int main(UNUSED int argc, UNUSED char *argv[]){
	pthread_t WorkerThread;
    pthread_create(&WorkerThread, NULL, &threadfunc, NULL);
    pthread_join(WorkerThread, NULL);
}