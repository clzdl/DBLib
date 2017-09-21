/*
 * ConnectionPool.cpp
 *
 *  Created on: 2017年9月1日
 *      Author: cplusplus
 */

#include "stdio.h"
#include "pool/ConnectionPool.h"

namespace DBLIB{

ConnectionPool* ConnectionPool::instance = NULL;

ConnectionPool::ConnectionPool(ConnectionFactory *connFactory,unsigned int cnt)
:m_connFactory(connFactory),
 m_uCnt(cnt),
 m_uUsedCnt(0)
{
}

ConnectionPool::~ConnectionPool()
{
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
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
	pthread_mutex_lock(&m_mutex);
	if(0 == m_pool.size() ){
		if(m_uUsedCnt == m_uCnt)
			pthread_cond_wait(&m_cond,&m_mutex);
		else
			m_pool.push_back(m_connFactory->create());  ///创建新连接
	}
	otl_connect *conn = NULL;
	do{
		conn = &(*(m_pool.back()));
		if(conn && !conn->connected)
		{
			delete conn;
			conn = NULL;
		}
		m_pool.pop_back();
	}while(!conn || m_pool.empty());

	if(NULL != conn)
		++m_uUsedCnt;

	pthread_mutex_unlock(&m_mutex);
	return conn;
}

void ConnectionPool::Release(otl_connect *conn)
{
	pthread_mutex_lock(&m_mutex);
	m_pool.push_back(conn);
	--m_uUsedCnt;
	pthread_cond_signal(&m_cond);
	pthread_mutex_unlock(&m_mutex);
}

void ConnectionPool::Initialize()
{
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_cond , NULL);
	for(unsigned int i = 0; i< 1 ; ++i ){
		m_pool.push_back(m_connFactory->create());
	}
}

}
