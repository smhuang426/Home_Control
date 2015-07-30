#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "../Library/Sha256/sha256.h"

/*define parameter*/
pthread_t thr1,thr2;

/*declare variable*/

/*declare function*/
void *thread_function(void* ptr);

void print_hash(unsigned char hash[])
{
   int idx;
   for (idx=0; idx < 32; idx++)
      printf("%02x",hash[idx]);
   printf("\n");
}


int main()
{
	unsigned char text1[]={"abc"},hash[32];
	int ret = 0;
	char* str1="test1",*str2="test2";

	SHA256_CTX ctx;
	sha256_init(&ctx);
   	sha256_update(&ctx,text1,strlen(text1));
   	sha256_final(&ctx,hash);
   	print_hash(hash);
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

