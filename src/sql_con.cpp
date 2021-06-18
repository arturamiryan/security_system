#include "sql_con.hpp"

SQL_Connection::SQL_Connection()
{
	struct connection_details mysqlD;
	mysqlD.server = "localhost";
	mysqlD.user = "albus";
	mysqlD.password = "123qwe";
	mysqlD.database = "company_staff";
	// connect to the mysql database
	connection = mysql_connection_setup(mysqlD);
	// get the results from executing commands
	res = mysql_perform_query(connection, "select * from employees where database_id=999");
	row = mysql_fetch_row(res);
}

SQL_Connection::~SQL_Connection()
{
	mysql_close(connection);
}

MYSQL* SQL_Connection::mysql_connection_setup(struct connection_details mysql_details)
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

MYSQL_RES* SQL_Connection::mysql_perform_query(MYSQL *connection, const string sql_query){
	//send query to db
	if(mysql_query(connection, sql_query.c_str()))
	{
		cout << "MySQL Query Error: " << mysql_error(connection) << endl;
		exit(1);
	}

	return mysql_use_result(connection);
}

void SQL_Connection::change_employee(int db_id)
{
	// clean up the database result
	mysql_free_result(res);
	string queryy = "select * from employees where database_id=";
	queryy.append(to_string(db_id));
	res = mysql_perform_query(connection, queryy);
	row = mysql_fetch_row(res);
}

std::string SQL_Connection::get_row(int r)
{
	return row[r];
}

unsigned int SQL_Connection::db_rows_num()
{
	mysql_free_result(res);

	res = mysql_perform_query(connection, "select count(*) from employees");

	row = mysql_fetch_row(res);

	return stoi(row[0]);
}
