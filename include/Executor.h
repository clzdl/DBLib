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
#include <string>
#include "otl/otlv4config.h"
#include <map>
namespace DBLib{

const int DEFAULT_BUFF_SIZE = 20;

//绑定具体参数字段
typedef void (*BindFunc)( otl_stream &stmt ,DbFieldBinder &field);
class Executor
{
public:
	struct ErrInfoMap
	{
		const char *strErrCode;
		const char *strErrRemark;
	};

	Executor(otl_connect *conn);
	~Executor();

	/**
	 * 打开查询流
	 */
	std::shared_ptr<otl_stream> Query(std::string sql , int buffSize = DEFAULT_BUFF_SIZE);

	/**
	 * 执行更新数据的sql
	 */
	std::shared_ptr<otl_stream> Execute(std::string sql  , int buffSize = 1 );

	/**
	 * 提取数据
	 */
	void FetchData(std::shared_ptr<otl_stream> stmt , _RESULT_ROW_VEC &result , int buffSize = DEFAULT_BUFF_SIZE);

	/**
	 * 按数据库原始类型数据进行提取
	 */
	void FetchOrgData(std::shared_ptr<otl_stream> stmt , _RESULT_ROW_VEC &result , int buffSize = DEFAULT_BUFF_SIZE);


	/**
	 * 绑定参数
	 */
	void BindParam(std::shared_ptr<otl_stream> otl_stmt , _BINDER_VEC &paramVec , bool bAutoFlush = true) ;


	template<typename T=DbFieldBinder, typename... Args>
	void BindParam(std::shared_ptr<otl_stream> otl_stmt,bool bAutoFlush,T binder ,Args... args)
	{
		try
		{
			bindParamRel[binder.iType](*otl_stmt,binder);
			if(sizeof...(args) > 0){
				BindParam(otl_stmt , bAutoFlush , std::forward(args...));
			}
			else
			{
				return ;
			}
		}
		catch(otl_exception& e)
		{
			if(e.code == ORA_PIPE_BREAK || e.code == ORA_DISCONNECT)
			{
				THROW(DBConnBreakException,ORA_BRK_EXP_MSG);
			}
			throw;
		}
	}

	/**
	 * 批量绑定参数(update/delete)，若期间抛异常，则跳过出错行继续执行
	 * errVec 接受出错行的行号
	 */
	void BatBindParam(std::shared_ptr<otl_stream> otl_stmt , std::vector<_BINDER_VEC> &mutiParamVec ,std::vector<size_t> *errVec = NULL) ;

	/**
	 * 事物提交
	 */
	static void Commit(otl_connect *conn);
	/**
	 * 事物回退
	 */
	static void Rollback(otl_connect *conn);

private:
	Executor(const Executor *exec);
	Executor& operator = (const Executor *exec);
	otl_connect *m_conn;

	static std::map<int , BindFunc> bindParamRel;



	static ErrInfoMap stErrInfoMap[];
};

}



#endif /* INCLUDE_EXECUTOR_H_ */
