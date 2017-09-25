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
#include <memory>
#include <thread>

#include "pool/ConnectionPool.h"
#include "Executor.h"


void threadFunc(int no)
{
	do{
		otl_connect *conn = DBLIB::ConnectionPool::GetInstance()->GetConnection();
		DBLIB::Executor executor(conn);
		std::shared_ptr<otl_stream> stmtPtr = executor.Query("select password from f_user");
		DBLIB::_ROW_VEC rowVec;
		for(;;)
		{
			executor.FetchData(stmtPtr ,rowVec);
			if(rowVec.empty())
				break;
			for(auto it = rowVec.begin(); it != rowVec.end(); ++it)
			{
				fprintf(stdout , "thread[%d]%s\n" , no,it->at(0)->fieldValue.strValue);
			}

			rowVec.clear();
			usleep(100);
		}

		DBLIB::ConnectionPool::GetInstance()->Release(conn);

	}while(1);
}


int main(int argc , char* argv[]){
	DBLIB::ConnectionPool::Initialize(new DBLIB::ConnectionFactory("UID=root;PWD=root;DSN=myodbc3"),1);
	std::thread t1(threadFunc,1);
	std::thread t2(threadFunc,2);
	std::thread t3(threadFunc,3);
	std::thread t4(threadFunc,4);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	fprintf(stdout,"run over.\n");
	return 0;
}






