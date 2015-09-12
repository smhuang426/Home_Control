#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include "../Library/Mysql/Ex_mysql.h"
#include "Ex_mysql.h"

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
	MYSQL *con = ex_mysql_init_with_db(NULL); 

	ex_mysql_list_db(con);

	if (ex_mysql_is_db_exist(con, "testdb")==EX_MYSQL_FAIL)
	{
		ex_mysql_create_db(con, "testdb");
	}else{
		ex_mysql_delete_db(con,"testdib2");
	}

  	ex_mysql_change_db_to(con, "testdb" , true);
	printf("current db is %s\n",ex_mysql_get_current_db());

	if (ex_mysql_is_table_exist_from_db(con, "test_table","testdb")!=EX_MYSQL_SUCCESS)
	{
		printf("creating table\n");
		ex_mysql_create_table_with_arg(con,"test_table","name char(20)", "no INT","description TEXT",NULL);
	}
	ex_mysql_show_tables_from_db(con, "testdb");

//	ex_mysql_insert_values_to_table(con, ex_mysql_get_current_db(),"test_table",/* "1",*/"'Noah2'","456","'no comment'",NULL);
	ex_mysql_delete_values_from_table_with_muticondition(con, ex_mysql_get_current_db(),"test_table", EX_MYSQL_AND , "sn=1",NULL);

	int num = ex_mysql_number_of_column_from_table(con , "test_table");
	printf("column number is %d\n",num);

	ex_mysql_select_col_from_table(con,"testdb" ,"test_table" ,"*", NULL);


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

