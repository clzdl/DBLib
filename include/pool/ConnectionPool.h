/*
 * ConnectionPool.h
 *
 *  Created on: 2017年9月1日
 *      Author: cplusplus
 */

#ifndef INCLUDE_POOL_CONNECTIONPOOL_H_
#define INCLUDE_POOL_CONNECTIONPOOL_H_

#include <ConnectionFactory.h>
#include "pthread.h"
#include <vector>

namespace DBLIB{

class ConnectionPool{
public:
	/**
	 * 初始化连接池
	 */
	static void Initialize(ConnectionFactory *factory,unsigned int maxSize = 4);

	/**
	 * 获取实例
	 */
	static ConnectionPool* GetInstance ();

	/**
	 * 获取连接
	 */
	otl_connect* GetConnection();

	/**
	 * 释放连接
	 */
	void Release(otl_connect *conn);

private:
	ConnectionPool(ConnectionFactory *connFactory,unsigned int cnt);
	~ConnectionPool();
	ConnectionPool(const ConnectionPool &cp);

	ConnectionPool& operator = (const ConnectionPool &cp);

	void Initialize();

	static ConnectionPool *instance;
	ConnectionFactory *m_connFactory;
	unsigned int m_uCnt;       //最大连接数
	std::vector<otl_connect*> m_pool;

	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
};


}



#endif /* INCLUDE_POOL_CONNECTIONPOOL_H_ */
