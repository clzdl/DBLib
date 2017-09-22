/*
 * ConnectionPool.h
 *
 *  Created on: 2017年9月1日
 *      Author: cplusplus
 */

#ifndef INCLUDE_POOL_CONNECTIONPOOL_H_
#define INCLUDE_POOL_CONNECTIONPOOL_H_

#include <ConnectionFactory.h>
#include <vector>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable


/**
 * 本想利用 otl的连接池，但查阅一边后不符合要求
 */

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
	 * 支持重连
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
	unsigned int m_uUsedCnt;   ///已使用的连接数
	std::vector<otl_connect*> m_pool;

	std::mutex m_mutex;
	std::condition_variable m_condition;

	static const int m_reConnTime = 3;    ///重连次数
	static const int m_reConnDelayTime = 1;   ///重连延迟 单位秒
};


}



#endif /* INCLUDE_POOL_CONNECTIONPOOL_H_ */