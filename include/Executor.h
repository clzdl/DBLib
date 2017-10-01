/*
 * Executor.h
 *
 *  Created on: 2017年9月15日
 *      Author: cplusplus
 */

#ifndef INCLUDE_EXECUTOR_H_
#define INCLUDE_EXECUTOR_H_

#include "DbFieldResult.h"
#include "DbFieldBinder.h"
#include "ErrCode.h"
#include <string>
#include "otl/otlv4config.h"
namespace DBLib{

const int DEFAULT_BUFF_SIZE = 20;


class Executor
{
public:
	Executor(otl_connect *conn);
	~Executor();

	/**
	 * 需要后续释放内存
	 */
	std::shared_ptr<otl_stream> Query(std::string sql , int buffSize = DEFAULT_BUFF_SIZE) throw();

	/**
	 * 执行更新数据的sql
	 */
	std::shared_ptr<otl_stream> Execute(std::string sql  , int buffSize = 1 ) throw();

	/**
	 * 提取数据
	 */
	_DBERROR FetchData(std::shared_ptr<otl_stream> stmt , _RESULT_ROW_VEC &result , int buffSize = DEFAULT_BUFF_SIZE) throw();

	/**
	 * 按数据库原始类型数据进行提取
	 */
	_DBERROR FetchOrgData(std::shared_ptr<otl_stream> stmt , _RESULT_ROW_VEC &result , int buffSize = DEFAULT_BUFF_SIZE) throw();


	/**
	 * 绑定参数
	 */
	void BindParam(std::shared_ptr<otl_stream> otl_stmt , _BINDER_VEC &paramVec , bool bAutoFlush = true) throw();

	/**
	 * 变长绑定参数,
	 * BindParam(otlPtr,2,DbFieldBinder(1111,111.11,"1111"),DbFieldBinder(1111,111.11,"1111"));
	 */
	void BindParam(std::shared_ptr<otl_stream> otl_stmt,int paramCnt , ...);


	/**
	 * 批量绑定参数(update/delete)，若期间抛异常，则跳过出错行继续执行
	 * errVec 接受出错行的行号
	 */
	void BatBindParam(std::shared_ptr<otl_stream> otl_stmt , std::vector<_BINDER_VEC> &mutiParamVec ,std::vector<size_t> *errVec = NULL) throw();


	static void Commit(otl_connect *conn) throw();

	static void Rollback(otl_connect *conn) throw();

	void BindParam(std::shared_ptr<otl_stream> otl_stmt,bool bAutoFlush,... );

private:
	Executor(const Executor *exec);
	Executor& operator = (const Executor *exec);
	otl_connect *m_conn;
};

}



#endif /* INCLUDE_EXECUTOR_H_ */
