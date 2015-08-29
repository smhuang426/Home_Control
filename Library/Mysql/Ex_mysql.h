#ifndef __EX_MYSQL_H__
#define __EX_MYSQL_H__
#include <mysql.h>

#define EX_LOCAL_HOST 	"localhost"
#define EX_USER		"root"
#define EX_PASSWORD	"OK5074"

typedef enum _EX_MYSQL_RES
{
	EX_MYSQL_SUCCESS = 0,
	EX_MYSQL_FAIL
}  EX_MYSQL_RES;


MYSQL *ex_mysql_init();
void ex_mysql_list_db(MYSQL *con);
EX_MYSQL_RES ex_mysql_is_db_exist(MYSQL *con, char* db_name);

#endif
