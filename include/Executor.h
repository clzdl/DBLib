/*
 * Executor.h
 *
 *  Created on: 2017年9月15日
 *      Author: cplusplus
 */

#ifndef INCLUDE_EXECUTOR_H_
#define INCLUDE_EXECUTOR_H_

#include "ErrCode.h"
#include <string>
#include "DbField.h"
#include "otl/otlv4config.h"
namespace DBLIB{

const int DEFAULT_BUFF_SIZE = 20;
typedef   std::vector<_ROW>  _ROW_VEC ;

class Executor
{
public:
	Executor(otl_connect *conn);
	~Executor();

	/**
	 * 需要后续释放内存
	 */
	std::shared_ptr<otl_stream> Query(std::string sql , int buffSize = DEFAULT_BUFF_SIZE);

	/**
	 * 执行更新数据的sql
	 */
	std::shared_ptr<otl_stream> Execute(std::string sql  , int buffSize = 1 );

	/**
	 * 提取数据
	 */
	_DBERROR FetchData(std::shared_ptr<otl_stream> stmt , _ROW_VEC &result , int buffSize = DEFAULT_BUFF_SIZE);

	/**
	 * 绑定参数
	 */
	void BindParam(std::shared_ptr<otl_stream> otl_stmt , _PARAM_VEC &paramVec , bool bAutoFlush = true);

	/**
	 * 批量绑定参数(update/delete)，若期间抛异常，则跳过出错行继续执行
	 * errVec 接受出错行的行号
	 */
	void BatBindParam(std::shared_ptr<otl_stream> otl_stmt , std::vector<_PARAM_VEC> &mutiParamVec ,std::vector<size_t> *errVec = NULL);

private:
	Executor(const Executor *exec);
	Executor& operator = (const Executor *exec);
	otl_connect *m_conn;
};

}



#endif /* INCLUDE_EXECUTOR_H_ */
