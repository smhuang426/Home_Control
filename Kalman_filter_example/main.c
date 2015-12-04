#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "kalman.h"

/*define parameter*/
pthread_t thr1;

/*declare variable*/
KM_1d_config config_km1;
KM_2d_config config_km2;

/*declare function*/
void *thread_function(void* ptr);

int main()
{
	int ret = 0;
	char* str1="test1";
    
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
    
    double result = 0,measure = 0, noise = 0;
    
    srand (time(NULL));
    
    KM_1d_init(&config_km1 ,0.1, 0.1);

    while (1) {
        noise = ((double)(rand()%100)-50)/250;
        measure =  1.0+noise;
        result = KM_1d_start(&config_km1, measure);
        printf("result :%f, measure:%f\n",result,measure);
        sleep(1);
    }
    
    return 0;
}

