#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../Library/Mysql/Ex_mysql.h"

#define LOCAL_HOST 	"localhost"
#define USER		"root"
#define PASSWORD	"OK5074"

/*define parameter*/
pthread_t thr1,thr2;

/*declare variable*/

/*declare function*/
void *thread_function(void* ptr);

int main()
{
	int ret = 0;
	char* str1="test1";

	printf("start connect to server\n");
	MYSQL *con = ex_mysql_init(); 

  	if (mysql_query(con, "CREATE DATABASE testdib2")) 
 	{
      		fprintf(stderr, "%s\n", mysql_error(con));
      		mysql_close(con);
      		exit(1);
  	}


	printf("MySQL client version: %s\n", mysql_get_client_info());

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
}

