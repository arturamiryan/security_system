#ifndef SQL_CONNECTION_H
#define SQL_CONNECTION_H

#include <mysql/mysql.h>
#include <string>
#include <iostream>

using namespace std;

class SQL_Connection
{
	protected:
		MYSQL *connection;
		MYSQL_RES *res;
		MYSQL_ROW row;
		unsigned int rows_num;
		struct connection_details{const char *server, *user, *password, *database;};
		MYSQL* mysql_connection_setup(struct connection_details mysql_details);
		MYSQL_RES* mysql_perform_query(MYSQL *connection, const string sql_query);
	
	public:
		SQL_Connection();
		virtual ~SQL_Connection();
		void change_employee(int db_id);
		string get_row(int r);
		unsigned int db_rows_num();
};

#endif //MYSQL_CONNECTION_H
