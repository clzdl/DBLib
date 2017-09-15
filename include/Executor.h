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
	otl_stream* Query(std::string sql , int buffSize = DEFAULT_BUFF_SIZE);

	_DBERROR FetchData(otl_stream* stmt , _ROW_VEC &result , int buffSize = DEFAULT_BUFF_SIZE);

private:
	Executor(const Executor *exec);
	Executor& operator = (const Executor *exec);
	otl_connect *m_conn;
};

}



#endif /* INCLUDE_EXECUTOR_H_ */
