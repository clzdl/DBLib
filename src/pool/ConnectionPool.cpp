/*
 * ConnectionPool.cpp
 *
 *  Created on: 2017年9月1日
 *      Author: cplusplus
 */

#include "stdio.h"
#include <unistd.h>
#include "pool/ConnectionPool.h"
#include "Defines.h"

namespace DBLib{

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

bool ConnectionPool::m_isInitilize = false;
ConnectionPool::ConnectionPool(ConnectionFactory *connFactory,unsigned int cnt)
:m_connFactory(connFactory),
 m_uCnt(cnt),
 m_uUsedCnt(0)
{
}

ConnectionPool::~ConnectionPool()
{
    for(auto it : m_pool)
    {
        RemoveConnection(it);
    }
}

ConnectionPool* ConnectionPool::Create(ConnectionFactory *factory,unsigned int maxSize)
{
	if(!m_isInitilize)
	{///进程启动初始，进行初始话，不考虑并行
		otl_connect::otl_initialize(1);
		m_isInitilize = true;
	}

	ConnectionPool *pool = new ConnectionPool(factory,maxSize);
	pool->Initialize();
	return pool;
}


otl_connect* ConnectionPool::GetConnection()
{
	otl_connect *conn = nullptr;
	int reConnTime = 0;
	std::unique_lock<std::mutex> lck(m_mutex);
	do{

		while(m_pool.empty())
		{

			if(m_uUsedCnt == m_uCnt)  ///池子已满
			{
				fprintf(stdout , "链接池已满，等待空闲链接.m_uUsedCnt:%d,m_uCnt%d\n",m_uUsedCnt,m_uCnt);
				m_condition.wait(lck);
			}
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
