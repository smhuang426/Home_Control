#include "Ex_mysql.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void finish_with_error(MYSQL *con);

MYSQL *ex_mysql_init(void)
{
	MYSQL *con = mysql_init(NULL);

	printf("mysql has initialized\n");  
  	if (con == NULL)
  	{
      		fprintf(stderr, "mysql_init() failed\n");
		exit(1);
  	}  
 
	printf("connect to server\n"); 
  	if (mysql_real_connect(con, EX_LOCAL_HOST, EX_USER, EX_PASSWORD, NULL, 0, NULL, 0) == NULL) 
  	{
	      	finish_with_error(con);
  	}

	return con;
}


void finish_with_error(MYSQL *con)
{
  	fprintf(stderr, "[ex_mysql error]%s\n", mysql_error(con));

  	mysql_close(con);

	exit(1);     

}

void ex_mysql_list_db(MYSQL *con)
{
	if (mysql_query(con, "show databases")) 
  	{
      		finish_with_error(con);
  	}

  	MYSQL_RES *result = mysql_store_result(con);
  
  	if (result == NULL) 
  	{
      		finish_with_error(con);
  	}

  	int num_fields = mysql_num_fields(result);

  	MYSQL_ROW row;
	int i = 0;

	printf("[ex_mysql_list_db]there are %d DBs as following below:\n"); 
  	while ((row = mysql_fetch_row(result))) 
  	{ 
      		for(i = 0; i < num_fields; i++) 
      		{ 
          		printf("	%s ", row[i] ? row[i] : "NULL"); 
      		} 
          		printf("\n"); 
  	}
	printf("[ex_mysql_list_db]end of db look up\n");

  	mysql_free_result(result);
}


EX_MYSQL_RES ex_mysql_is_db_exist(MYSQL *con, char* db_name)
{
	EX_MYSQL_RES ret = EX_MYSQL_FAIL;

	if (mysql_query(con,"show databases")) 
  	{
      		return ret;
  	}

  	MYSQL_RES *result = mysql_store_result(con);
  
  	if (result == NULL) 
  	{
		return ret;
  	}

  	int num_fields = mysql_num_fields(result);

  	MYSQL_ROW row;
	int i = 0;

  	while ((row = mysql_fetch_row(result))) 
  	{ 
      		for(i = 0; i < num_fields; i++) 
      		{ 
          		if ((strcmp( row[i] , db_name)==0))
			{
				return EX_MYSQL_SUCCESS;
			} 
      		} 
  	}

  	mysql_free_result(result);
	return ret;
}


