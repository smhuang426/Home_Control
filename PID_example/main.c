#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "PID.h"

/*define parameter*/
pthread_t thr1;

/*declare variable*/
CONTROLLER pid_config,pid_config2;
float output=0,feedback=0,output2=0,feedback2=0;

/*declare function*/
void *thread_function(void* ptr);

int main()
{
	int ret = 0;
	char* str1="test1";

    PID_setup_controller(&pid_config, 0.4 , 0.4, 0.0 , 1.0, 10.0);
    PID_setup_controller(&pid_config2, 0.0 , 1.2, 0.1 , 1.0, 10.0);
    
    PID_activate(&pid_config, 8, &feedback);
    PID_activate(&pid_config2, 8, &feedback2);
    printf("expected output:%f\n",pid_config.Expected_output);
    while (1) {
        PID_activate(&pid_config, feedback, &output);
        PID_activate(&pid_config2, feedback2, &output2);
        printf("output:  %f, feedback: %f\n",output,feedback);
        printf("output2: %f, feedback2:%f\n\n",output2,feedback2);
        feedback = output;
        feedback2 = output2;
        sleep(1);
    }

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

