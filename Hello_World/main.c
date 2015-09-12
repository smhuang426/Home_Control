//#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*define parameter*/
pthread_t thr1,thr2;

/*declare variable*/

/*declare function*/
void *thread_function(void* ptr);

int function (int from , int to);

int a[4]={0,1,1,1};

typedef enum BOOL{
    false = 0,
    true  = 1
} BOOL;

int isBadVersion(int version)
{
    if (a[version] == 1)
    {
        return 1;
    }
    
    else{
        return 0;
    }
}

int firstBadVersion(int n) {
    
    if (isBadVersion(1) == true)    return 1;
    if (isBadVersion(n) == true)    return n;
    
    return function(1,n);
}

int function (int from , int to){
    
    int _temp = to - from;
    int temp = _temp/2 + from;
    BOOL last_chance = false;
    int ret = 0;
    
    if (_temp <= 2)
    {
        last_chance = true;
    }
    
    if (isBadVersion(temp) != true){
        
        if (last_chance == true){
            ret = to;
        }else{
        
            function (from , temp);
        }
        
    }else{
        
        if (last_chance == true){
            ret = temp;
        }else{
        
            function (temp, to);
        }
    }
    
    return ret;
    
}

int main()
{
	int ret = 0;
	char* str1="test1",*str2="test2";
    
    printf("ret:%d\n",/*firstBadVersion(3)*/(int)1.5 );

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
    
    return 0;
}

