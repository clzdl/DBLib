/*
 * mysql-odbc.cpp
 *
 *  Created on: 2017年9月16日
 *      Author: cplusplus
 */


/*
 * main.cpp
 *
 *  Created on: 2017年9月4日
 *      Author: cplusplus
 */

#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>

#include "pool/ConnectionPool.h"
#include "Executor.h"

int main(int argc , char* argv[]){
	DBLIB::ConnectionPool::Initialize(new DBLIB::ConnectionFactory("UID=root;PWD=root;DSN=myodbc3"));
	otl_connect *conn = DBLIB::ConnectionPool::GetInstance()->GetConnection();
	DBLIB::Executor executor(conn);
	otl_stream *stmt = executor.Query("select password from f_user");
	DBLIB::_ROW_VEC rowVec;
	executor.FetchData(stmt ,rowVec);

	for(auto it = rowVec.begin(); it != rowVec.end(); ++it)
	{
		fprintf(stdout , "===%s\n" , it->at(0)->fieldValue.strValue);
	}

	return 0;
}






