#ifndef __EX_MYSQL_H__
#define __EX_MYSQL_H__
#include <mysql.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#define EX_LOCAL_HOST 	"localhost"
#define EX_USER		"root"
#define EX_PASSWORD	"OK5074"

typedef enum _EX_MYSQL_RES
{
	EX_MYSQL_SUCCESS = 0,
	EX_MYSQL_FAIL
}  EX_MYSQL_RES;


MYSQL *ex_mysql_init_with_db(char* _db);
void ex_mysql_list_db(MYSQL *con);
char* ex_mysql_get_current_db(void);
EX_MYSQL_RES ex_mysql_is_db_exist(MYSQL *con, char* db_name);
EX_MYSQL_RES ex_mysql_create_db(MYSQL* con, char* db_name);
EX_MYSQL_RES ex_mysql_delete_db(MYSQL* con, char* db_name);
EX_MYSQL_RES ex_mysql_change_db_to(MYSQL* con, char* db_name, uint8_t is_create);
EX_MYSQL_RES ex_mysql_show_tables_from_db(MYSQL* con, char* _db);
EX_MYSQL_RES ex_mysql_is_table_exist_from_db(MYSQL *con, char* table_name,char* _db);
EX_MYSQL_RES ex_mysql_create_table_with_arg(MYSQL* con, char* db_name, ...);
EX_MYSQL_RES ex_mysql_delete_table(MYSQL* con, char* table_name);
EX_MYSQL_RES ex_mysql_select_col_from_table(MYSQL* con, char* _db ,char* _table ,char* column, ...);


#endif
