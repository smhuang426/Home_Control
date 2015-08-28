#include "Ex_mysql.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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


