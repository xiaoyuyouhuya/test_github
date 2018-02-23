#include "thread_pool.h"
#include <stdio.h>
#include <pthread.h>


void *consumer(void *arg)
{
	FILE* fstream;
	if(fstream = fopen("./data", "at")){
		fprintf(fstream, "aaaaaadoing tasksaaaaaaaaa  : %d\n", pthread_self());
		usleep(1000);
	}
	else{
		fprintf(stderr, "Open error!\n");
	}
	fclose(fstream);
	return NULL;
}

void producer(void)
{
	int n = 0;
	while(1){

		FILE* fstream;
		if(fstream = fopen("./data", "at")){
			fprintf(fstream, "---------------------------------------------\n");
		}
		fclose(fstream);
		int i;
		for(i=1; i<=10; i++)
		{
			thread_pool_add_task(consumer,(void *)i);
			usleep(1000);
		}
		sleep(5);
		fprintf(stderr, "---------------------\n");
		if(5 == n) break;
		n++;
	}
}


int producer_consumer_model()
{

	thread_pool_init(5);
	pthread_t id;
	int i,ret;
	ret=pthread_create(&id,NULL,(void *)producer,NULL);
	if(ret != 0){
		printf ("Create pthread error!\n");
		exit (1);
	}
	pthread_join(id,NULL);
	thread_pool_destroy();
	return 0;
}
