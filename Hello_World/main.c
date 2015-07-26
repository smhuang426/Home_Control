#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*define parameter*/
pthread_t thr1,thr2;

/*declare variable*/

/*declare function*/
void *thread_function(void* ptr);

int main()
{
	int ret = 0;
	char* str1="test1",*str2="test2";

	printf("hello world\n");

	ret = pthread_create(&thr1,NULL,thread_function,str1);
	if (ret !=0)
	{
		printf("thread create fail");
	}

	ret = pthread_create(&thr2,NULL,thread_function,str2);
	if (ret != 0)
	{
		printf("thread create fail");
	}

	pthread_join(thr1,NULL);
	pthread_join(thr2,NULL);

	//while loop
	while(1);
	return 0;


}

void *thread_function(void* ptr)
{
	char *str;
	str = (char *)ptr;
	printf("pthread_t:%s\n",str);
}

