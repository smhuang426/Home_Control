#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "FIR.h"

/*define parameter*/
pthread_t thr1;

/*declare variable*/
FIR_CONFIG config;

/*declare function*/
void *thread_function(void* ptr);

int main()
{
	int ret = 0;
	char* str1="test1";

    FIR_init(&config, HIGHPASS, HANN, 10, 20000, 2500);
    printf("\nstart:%f\n",FIR_start_filter(&config, 1.0));
    //FIR_start_filter(&config, 0.0);
    
    FIR_deinit(&config);

	ret = pthread_create(&thr1,NULL,thread_function,str1);
	if (ret !=0)
	{
		printf("thread create fail");
	}

	pthread_join(thr1,NULL);

	//while loop
	while(1);
	return 0;


}

void *thread_function(void* ptr)
{
	char *str;
	str = (char *)ptr;
	printf("pthread_t:%s\n",str);
    

    
    return 0;
}

