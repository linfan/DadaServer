/*
 * connection_pool.cpp
 *
 *  Created on: 2013-3-29
 *      Author: csc
 */

#include <stdexcept>
#include <exception>
#include <stdio.h>
#include "connection_pool.h"

using namespace std;
using namespace sql;

ConnPool *ConnPool::connPool = NULL;

// Constructor of the cnnection pool
ConnPool::ConnPool(string url, string userName, string password, int maxSize) {
	this->maxSize = maxSize;
	this->curSize = 0;
	this->username = userName;
	this->password = password;
	this->url = url;
	try {
		this->driver = sql::mysql::get_driver_instance();
	} catch (sql::SQLException&e) {
		perror("SQL driver error.\n");
	} catch (std::runtime_error&e) {
		perror("SQL runtime error.\n");
	}
	this->InitConnection(maxSize / 2);
}

// Get the connection pool singleton instance
ConnPool*ConnPool::GetInstance() {
	if (connPool == NULL) {
		connPool = new ConnPool("tcp://127.0.0.1:3306",
                CONNPOOL_USER, CONNPOOL_PASSWORD, CONNPOOL_POOLSIZE);
	}
	return connPool;
}

// Initialize connection pool, create half of the maximum connection number
void ConnPool::InitConnection(int iInitialSize) {
	Connection*conn;
	pthread_mutex_lock(&lock);
	for (int i = 0; i < iInitialSize; i++) {
		conn = this->CreateConnection();
		if (conn) {
			connList.push_back(conn);
			++(this->curSize);
		} else {
			perror("Create connection error.\n");
		}
	}
	pthread_mutex_unlock(&lock);
}

// Create a new connection
Connection* ConnPool::CreateConnection() {
	Connection*conn;
	try {
        // Do connect
		conn = driver->connect(this->url, this->username, this->password);
		return conn;
	} catch (sql::SQLException&e) {
		perror("Connection creation error.\n");
		return NULL;
	} catch (std::runtime_error&e) {
		perror("Connection creation runtime error.\n");
		return NULL;
	}
}

// Acquire a connection from pool
Connection*ConnPool::GetConnection() {
	Connection*con;
	pthread_mutex_lock(&lock);

	if (connList.size() > 0) {	// There are available connection
		con = connList.front(); // Fetch first connection from list
		connList.pop_front(); 	// Remove the fetched connection
		if (con->isClosed()) {	// If connection force closed, reconnect
			delete con;
			con = this->CreateConnection();
		}
		// Reduce establed connection size if connection is NULL
		if (con == NULL) {
			--curSize;
		}
		pthread_mutex_unlock(&lock);
		return con;
	} else {
		if (curSize < maxSize) { // More connection can be created
			con = this->CreateConnection();
			if (con) {
				++curSize;
				pthread_mutex_unlock(&lock);
				return con;
			} else {
				pthread_mutex_unlock(&lock);
				return NULL;
			}
		} else { // Reached the maximum size
			pthread_mutex_unlock(&lock);
			return NULL;
		}
	}
}

// Release the connection back to idle list
void ConnPool::ReleaseConnection(sql::Connection * conn) {
	if (conn) {
		pthread_mutex_lock(&lock);
		connList.push_back(conn);
		pthread_mutex_unlock(&lock);
	}
}

// Destructor of connection pool
ConnPool::~ConnPool() {
	this->DestoryConnPool();
}

// Destory pool, disconnect all connection first
void ConnPool::DestoryConnPool() {
	list<Connection*>::iterator icon;
	pthread_mutex_lock(&lock);
	for (icon = connList.begin(); icon != connList.end(); ++icon) {
		this->DestoryConnection(*icon); // Disconnect it
	}
	curSize = 0;
	connList.clear(); // Clean the idle list
	pthread_mutex_unlock(&lock);
}

// Disconnect a connection
void ConnPool::DestoryConnection(Connection* conn) {
	if (conn) {
		try {
			conn->close();
		} catch (sql::SQLException&e) {
			perror(e.what());
		} catch (std::exception&e) {
			perror(e.what());
		}
		delete conn;
	}
}

