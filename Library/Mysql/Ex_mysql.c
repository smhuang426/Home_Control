#include "Ex_mysql.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Variable*/
char *current_db=NULL;

/*private function*/
void finish_with_error(MYSQL *con);
void set_current_db(char* _db);
void deinit_current_db(void);
int print_result(MYSQL* con);

/*implement function*/
void set_current_db(char* _db)
{
	if (current_db == NULL)
	{
		current_db = (char*)malloc(256*sizeof(char));
		printf("current_db is under init\n");
	}

	if((_db == NULL) || (strlen(_db) <= 0)) return ;

	memset(current_db,'\0',strlen(current_db));
	strcpy(current_db, _db);
}

void deinit_current_db(void)
{
	free(current_db);
}

char* ex_mysql_get_current_db(void)
{
	return current_db;
}

MYSQL *ex_mysql_init_with_db(char* _db)
{
	MYSQL *con = mysql_init(NULL);

	printf("mysql has initialized\n");  
  	if (con == NULL)
  	{
      		fprintf(stderr, "mysql_init() failed\n");
		//exit(1);
  	}  
 
	printf("connect to server\n"); 
  	if (mysql_real_connect(con, EX_LOCAL_HOST, EX_USER, EX_PASSWORD, _db, 0, NULL, 0) == NULL) 
  	{
	      	finish_with_error(con);
  	}

	set_current_db(_db);

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

	printf("[ex_mysql_list_db]there are DBs as following below:\n"); 

	print_result(con);

	printf("[ex_mysql_list_db]end of db look up\n");
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


EX_MYSQL_RES ex_mysql_create_db(MYSQL* con, char* db_name)
{
	char *create_db = (char*)malloc(256*sizeof(char));
	if (create_db == NULL)
	{
		return EX_MYSQL_FAIL;
	}

	strcpy(create_db,"create database ");
	strcat(create_db, db_name);

	if (mysql_query(con, create_db))
	{
		free(create_db);
      		fprintf(stderr, "%s\n", mysql_error(con));
		return EX_MYSQL_FAIL;
	}

	free(create_db);

	return EX_MYSQL_SUCCESS;
}

EX_MYSQL_RES ex_mysql_delete_db(MYSQL* con, char* db_name)
{
	if (con == NULL)
	{
		printf("MYSQL not be connected\n");
		return EX_MYSQL_FAIL;
	}

	char *create_db = (char*)malloc(256*sizeof(char));
	if (create_db == NULL)
	{
		return EX_MYSQL_FAIL;
	}

	strcpy(create_db,"drop database ");
	strcat(create_db, db_name);

	if (mysql_query(con, create_db))
	{
		free(create_db);
      		fprintf(stderr, "%s\n", mysql_error(con));
		return EX_MYSQL_FAIL;
	}

	free(create_db);

	return EX_MYSQL_SUCCESS;

}

EX_MYSQL_RES ex_mysql_change_db_to(MYSQL* con, char* db_name, uint8_t is_create)
{
	EX_MYSQL_RES ret = EX_MYSQL_SUCCESS;

	if (ex_mysql_is_db_exist(con, db_name) == EX_MYSQL_SUCCESS)
	{
		char* _db = (char*)malloc(256*sizeof(char));
		strcpy(_db, "use ");
		strcat(_db, db_name);
		printf("change to database :%s\n",db_name);
		if (mysql_query(con, _db))
		{
			printf("change to existed db is fail\n");
			ret = EX_MYSQL_FAIL;
		}
		free(_db);
	}else{
		if (is_create == true){
			printf("%s is not exist , creating\n",db_name);
			if (ex_mysql_create_db(con, db_name) != EX_MYSQL_SUCCESS)
			{
				printf("database create fial\n");
				ret = EX_MYSQL_FAIL;
			}
		}
		else{
			printf("database is not exist\n");
			ret = EX_MYSQL_FAIL;
		}
	}

	if (ret == EX_MYSQL_SUCCESS)
	{
		set_current_db(db_name);
	}

	return ret;
}


EX_MYSQL_RES ex_mysql_show_tables_from_db(MYSQL* con, char* _db)
{
	EX_MYSQL_RES ret = EX_MYSQL_FAIL;
	char *show_table = (char*)malloc(256*sizeof(char));

	strcpy(show_table,"show tables from ");
	strcat(show_table,_db);

	if (mysql_query(con, show_table))
	{
		return ret;
	}

	printf("[ex_mysql_show_tables_from_db]there are tables as following below\n");

	print_result(con);

	printf("[ex_mysql_show_tables_from_db]end of lookup tables\n");	

	return ret;
}

int print_result(MYSQL* con)
{
  	MYSQL_RES *result = mysql_store_result(con);

  	if (result == NULL) 
  	{
      		return ;
  	}  

  	int num_fields = mysql_num_fields(result),i=0;

	MYSQL_ROW row;
 	MYSQL_FIELD *field;
	int number = 0;

	printf("\n------------\n");           

  	while ((row = mysql_fetch_row(result))) 
  	{ 
      		for(i = 0; i < num_fields; i++) 
      		{ 
          		if (i == 0) 
			{              
				while(field = mysql_fetch_field(result)) 
             			{
                			printf("%s  ", field->name);
				//	printf("\n------------");
             			}
             
             			printf("\n------------\n");           
          		}
          
          		printf("%s  ", row[i] ? row[i] : "NULL"); 
			number = atoi(row[i]);
      		} 
  	}
  
  	printf("\n------------\n");
	mysql_free_result(result);
	return number;
}

EX_MYSQL_RES ex_mysql_is_table_exist_from_db(MYSQL *con, char* table_name,char* _db)
{
	EX_MYSQL_RES ret = EX_MYSQL_FAIL;
	char *show_table = (char*)malloc(256*sizeof(char));

	strcpy(show_table,"show tables from ");
	strcat(show_table,_db);

	if (mysql_query(con, show_table))
	{
		return ret;
	}

	MYSQL_RES *result = mysql_store_result(con);
	if (result == NULL) 
	{
		return ret;
  	}

	int num_fields = mysql_num_fields(result),i=0;

	MYSQL_ROW row;

	while ((row = mysql_fetch_row(result))) 
	{ 
    		for(i = 0; i < num_fields; i++) 
    		{ 
        		if (strcmp(row[i], table_name) == 0)
			{
				return EX_MYSQL_SUCCESS;
			}
    		} 
        	 
	}

	//print_result(con);
	
	return ret;
}

EX_MYSQL_RES ex_mysql_create_table_with_arg(MYSQL* con, char* db_name, char* pArg, ...)
{
	va_list arg_ptr;
	char* create_table,*str;

	create_table = (char*)malloc(512*sizeof(char));
	strcpy(create_table,"create table ");
	strcat(create_table, db_name);
	strcat(create_table,"(");
    	strcat(create_table, pArg);

	va_start(arg_ptr,pArg);

	str = va_arg(arg_ptr,char*);
	do
	{
		strcat(create_table,",");
		strcat(create_table,str);
		str=va_arg(arg_ptr,char*);
	}while (str != NULL);

	strcat(create_table,")");
	va_end(arg_ptr);

	printf("[mysql table create] %s\n",create_table);

	if (mysql_query(con, create_table))
	{
		free(create_table);
		fprintf(stderr, "[mysql table create]%s\n", mysql_error(con));
		return EX_MYSQL_FAIL;
	}

	free(create_table);

	return EX_MYSQL_SUCCESS;
}

EX_MYSQL_RES ex_mysql_delete_table(MYSQL* con, char* table_name)
{
	if (con == NULL)
	{
		printf("MYSQL not be connected\n");
		return EX_MYSQL_FAIL;
	}

	char *drop_table = (char*)malloc(256*sizeof(char));
	if (drop_table == NULL)
	{
		return EX_MYSQL_FAIL;
	}

	strcpy(drop_table,"drop table ");
	strcat(drop_table, table_name);

	if (mysql_query(con, drop_table))
	{
		free(drop_table);
      		fprintf(stderr, "%s\n", mysql_error(con));
		return EX_MYSQL_FAIL;
	}

	free(drop_table);

	return EX_MYSQL_SUCCESS;

}

EX_MYSQL_RES ex_mysql_select_col_from_table(MYSQL* con, char* _db ,char* _table ,char* column, ...)
{
	EX_MYSQL_RES ret;

	ret = ex_mysql_change_db_to(con, _db, false);
	if (ret != EX_MYSQL_SUCCESS)
	{
		printf("[list_data_from_table]database:%s is not exist\n",_db);
		return ret;
	}

	va_list arg_ptr;
	char* select_table,*str;

	select_table = (char*)malloc(512*sizeof(char));
	strcpy(select_table,"select ");
	strcat(select_table, column);

	va_start(arg_ptr,column);

	str = va_arg(arg_ptr,char*);
	while(str != NULL)
	{
		strcat(select_table,",");
		strcat(select_table,str);
		str=va_arg(arg_ptr,char*);
	}

	va_end(arg_ptr);

	strcat(select_table," from ");
	strcat(select_table, _table);
	printf("[select table]%s\n",select_table);
	if (mysql_query(con, select_table))
	{
		free(select_table);
      		fprintf(stderr, "%s\n", mysql_error(con));
		return EX_MYSQL_FAIL;
	}
	free(select_table);

	print_result(con);

	return EX_MYSQL_SUCCESS;
}


EX_MYSQL_RES ex_mysql_insert_values_to_table(MYSQL* con, char* _db,char* _table, char* values, ...)
{
	EX_MYSQL_RES ret;

	ret = ex_mysql_change_db_to(con, _db, false);
	if (ret != EX_MYSQL_SUCCESS)
	{
		printf("[insert_data_to_table]database:%s is not exist\n",_db);
		return ret;
	}

	va_list arg_ptr;
	char* insert_table,*str/*,str_num[25]*/;
	//int sn =  ex_mysql_number_of_column_from_table(con , _table) + 1;

	//sprintf(str_num, "%d", sn);

	insert_table = (char*)malloc(512*sizeof(char));
	strcpy(insert_table,"insert into ");
	strcat(insert_table, _table);
	strcat(insert_table," values (");
	//strcat(insert_table, str_num);
	//strcat(insert_table,",");	
	strcat(insert_table, values);

	va_start(arg_ptr,values);

	str = va_arg(arg_ptr,char*);
	while(str != NULL)
	{
		strcat(insert_table,",");
		strcat(insert_table,str);
		str=va_arg(arg_ptr,char*);
	}

	va_end(arg_ptr);

	strcat(insert_table,")");
	printf("[insert table]%s\n",insert_table);
	if (mysql_query(con, insert_table))
	{
		free(insert_table);
      		fprintf(stderr, "%s\n", mysql_error(con));
		return EX_MYSQL_FAIL;
	}
	free(insert_table);

	print_result(con);

	return EX_MYSQL_SUCCESS;
}

EX_MYSQL_RES ex_mysql_delete_values_from_table_with_muticondition(MYSQL* con, char* _db,char* _table, EX_MYSQL_AND_OR and_or,char* condition, ...)
{
	EX_MYSQL_RES ret;

	ret = ex_mysql_change_db_to(con, _db, false);
	if (ret != EX_MYSQL_SUCCESS)
	{
		printf("[delete_data_from_table]database:%s is not exist\n",_db);
		return ret;
	}

	va_list arg_ptr;
	char* delete_table,*str,logic[6];

	if (and_or == EX_MYSQL_AND) strcpy(logic," and ");
	else strcpy(logic," or ");

	delete_table = (char*)malloc(512*sizeof(char));
	strcpy(delete_table,"delete from ");
	strcat(delete_table, _table);
	strcat(delete_table," where ");
	strcat(delete_table, condition);

	va_start(arg_ptr,condition);

	str = va_arg(arg_ptr,char*);
	while(str != NULL)
	{
		strcat(delete_table,logic);
		strcat(delete_table,str);
		str=va_arg(arg_ptr,char*);
	}

	va_end(arg_ptr);

	printf("[delete table]%s\n",delete_table);
	if (mysql_query(con, delete_table))
	{
		free(delete_table);
      		fprintf(stderr, "%s\n", mysql_error(con));
		return EX_MYSQL_FAIL;
	}
	free(delete_table);

	print_result(con);

	return EX_MYSQL_SUCCESS;

}


int ex_mysql_number_of_column_from_table(MYSQL *con , char* table)
{
	char* query = (char*)malloc(256*sizeof(char));
	int number = 0;

	strcpy(query,"select COUNT(*) from ");
	strcat(query, table);

	if (mysql_query(con, query))
	{
		free(query);
      		fprintf(stderr, "%s\n", mysql_error(con));
		return number;
	}

	number = print_result(con);

	free(query);

	return number;
}

