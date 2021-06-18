#ifndef SOME_SQL_H
#define SOME_SQL_H

#include <mysql/mysql.h>
#include <iostream>

using namespace std;

struct connection_details{
	const char *server, *user, *password, *database;
};

MYSQL* mysql_connection_setup(struct connection_details mysql_details)
{
	MYSQL *connection = mysql_init(NULL); // mysql instance

	//connect database
	if(!mysql_real_connect(connection, mysql_details.server, mysql_details.user, mysql_details.password, mysql_details.database, 0, NULL, 0))
	{
		cout << "Connection Error: " << mysql_error(connection) << endl;
		exit(1); 
	}

	return connection;
}

MYSQL_RES* mysql_perform_query(MYSQL *connection, const string sql_query){
	//send query to db
	if(mysql_query(connection, sql_query.c_str()))
	{
		cout << "MySQL Query Error: " << mysql_error(connection) << endl;
		exit(1);
	}

	return mysql_use_result(connection);
}

#endif
