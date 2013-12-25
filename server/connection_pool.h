/*
 *File: connection_pool.h
 *Author: csc
 */

#ifndef _CONNECTION_POOL_H
#define	_CONNECTION_POOL_H

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <pthread.h>
#include <list>

using namespace std;
using namespace sql;

const string CONNPOOL_USER = "dada";
const string CONNPOOL_PASSWORD = "dada";
const int CONNPOOL_POOLSIZE = 10;

class ConnPool {
private:
	int curSize; // Current established connection count
	int maxSize; // Maximum established connection count
	string username;
	string password;
	string url;
	list<Connection*> connList; // Connection pool container
	pthread_mutex_t lock; // Mutex lock for connList access
	static ConnPool *connPool;
	Driver*driver;

	Connection*CreateConnection(); // Create a database connection
	void InitConnection(int iInitialSize); // Initialize database connection
	void DestoryConnection(Connection *conn); // Destory database connection
	void DestoryConnPool(); // Destory whole connection pool
	ConnPool(string url, string user, string password, int maxSize); // Constructor
public:
	~ConnPool();
	Connection*GetConnection(); // Acquire connection object
	void ReleaseConnection(Connection *conn); // Put used connection object back to container
	static ConnPool *GetInstance(); // Acquire connection pool singleton instance
};
#endif	/*_CONNECTION_POOL_H */
