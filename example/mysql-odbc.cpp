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


void threadFunc(std::string phone)
{
	do{
		otl_connect *conn = DBLIB::ConnectionPool::GetInstance()->GetConnection();
		DBLIB::Executor executor(conn);
		std::shared_ptr<otl_stream> stmtPtr = executor.Query("select phone,password from f_user where phone = :v1<char[20]>");

		DBLIB::_RESULT_ROW_VEC rowVec;
		DBLIB::_BINDER_VEC binderVec;

		DBLIB::DbFieldBinderHelper::BuildBinder4String(binderVec , const_cast<char*>(phone.c_str()));

		executor.BindParam(stmtPtr , binderVec);

		executor.FetchData(stmtPtr ,rowVec);
		if(rowVec.empty())
			break;
		for(auto it = rowVec.begin(); it != rowVec.end(); ++it)
		{
			fprintf(stdout , "%s,%s\n" , it->at(0)->fieldValue.strValue,it->at(1)->fieldValue.strValue);
		}

		usleep(100);

		DBLIB::ConnectionPool::GetInstance()->Release(conn);

	}while(1);
}


int main(int argc , char* argv[]){
	DBLIB::ConnectionPool::Initialize(new DBLIB::ConnectionFactory("UID=root;PWD=root;DSN=myodbc3"));
	std::thread t1(threadFunc,"18645005420");
	std::thread t2(threadFunc,"18731173110");
	std::thread t3(threadFunc,"18611628964");
	std::thread t4(threadFunc,"18618488857");

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	fprintf(stdout,"run over.\n");
	return 0;
}






