/*
 * ConnectionPool.cpp
 *
 *  Created on: 2017年9月1日
 *      Author: cplusplus
 */

#include <ExceptionDef.h>
#include "stdio.h"
#include <unistd.h>
#include "pool/ConnectionPool.h"

namespace DBLib{

ConnectionPool* ConnectionPool::instance = NULL;

static void RemoveConnection(otl_connect *conn)
{
	try
	{
		conn->rollback();
		conn->logoff();
	}catch (otl_exception &e) {
		_DUMP_EXCEPTION(e);
	}
	delete conn;
	conn = NULL;
}

ConnectionPool::ConnectionPool(ConnectionFactory *connFactory,unsigned int cnt)
:m_connFactory(connFactory),
 m_uCnt(cnt),
 m_uUsedCnt(0)
{
}

ConnectionPool::~ConnectionPool()
{
}

void ConnectionPool::Initialize(ConnectionFactory *connFactory , unsigned int maxSize)
{
	if(NULL == instance ){
		otl_connect::otl_initialize(1);
		instance = new ConnectionPool(connFactory,maxSize);
		instance->Initialize();
	}
}

ConnectionPool* ConnectionPool::GetInstance()
{
	if(NULL == instance){
		fprintf(stdout , "please first invoke function Initialize\n");
		return NULL;
	}
	return instance;
}


otl_connect* ConnectionPool::GetConnection()
{
	otl_connect *conn = NULL;
	int reConnTime = 0;
	std::unique_lock<std::mutex> lck(m_mutex);
	do{
		while(m_pool.empty())
		{
			if(m_uUsedCnt == m_uCnt)  ///池子已满
				m_condition.wait(lck);
			else
			{
				m_pool.push_back(m_connFactory->create());  ///创建新连接
				break;
			}
		}

		conn = m_pool.back();
		m_pool.pop_back();
		if(conn && !conn->connected)
		{
			RemoveConnection(conn);
			sleep(m_reConnDelayTime);
		}
		else
			++m_uUsedCnt;

	}while(!conn && m_reConnTime > reConnTime++);

	return conn;
}

void ConnectionPool::Release(otl_connect *conn)
{
	std::unique_lock<std::mutex> lck(m_mutex);
	m_pool.push_back(conn);
	--m_uUsedCnt;
	m_condition.notify_one();
}

void ConnectionPool::Initialize()
{
	for(unsigned int i = 0; i< 1 ; ++i ){
		m_pool.push_back(m_connFactory->create());
	}
}

}
