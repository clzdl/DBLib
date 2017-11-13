/*
 * Executor.cpp
 *
 *  Created on: 2017年9月15日
 *      Author: cplusplus
 */


#include "Executor.h"

namespace DBLib{


//获取结果集的具体字段
typedef _DBERROR (*AssFieldFunc)( otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal);



#define _STRERROR_GEN(t1,t2,t3)        { t2, t3 },
Executor::ErrInfoMap Executor::stErrInfoMap[] = {
		_ERR_MAP(_STRERROR_GEN)
};

#undef _STRERROR_GEN


const char* Executor::Errno2String(_DBERROR err)
{
    return stErrInfoMap[err].strErrRemark;
}

static _DBERROR AssField4VarChar(otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal)
{
	field->fieldValue.strValue = new char[desc.dbsize+1]();
	field->iType = FIELD_STRING;
	stmt>>field->fieldValue.strValue;
	return E_OK;
}

static _DBERROR AssField4Double( otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal)
{
	if(isOriginal )
	{  ///直接提取数据库原始值
		field->iType = FIELD_DOUBLE;
		stmt>>field->fieldValue.dValue;
	}
	else
	{
		if(desc.scale == 0 )
		{
			field->iType = FIELD_LONG;
			stmt>>field->fieldValue.lValue;
		}
		else
		{
			field->iType = FIELD_DOUBLE;
			stmt>>field->fieldValue.dValue;
		}
	}
	if(stmt.is_null() && field->iType == FIELD_DOUBLE)
		field->fieldValue.dValue = 0.0;

	return E_OK;
}


static _DBERROR AssField4Float( otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal)
{
	if(isOriginal )
	{   ///直接提取数据库原始值
		field->iType = FIELD_FLOAT;
		stmt>>field->fieldValue.fValue;
	}
	else
	{
		if(desc.scale == 0 )
		{
			field->iType = FIELD_INT;
			stmt>>field->fieldValue.iValue;
		}
		else
		{
			field->iType = FIELD_FLOAT;
			stmt>>field->fieldValue.fValue;
		}
	}
	if(stmt.is_null() && field->iType == FIELD_FLOAT)
		field->fieldValue.fValue = 0.0;

	return E_OK;
}

static _DBERROR AssField4Int( otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal)
{
	field->iType = FIELD_INT;
	stmt>>field->fieldValue.iValue;
	if(stmt.is_null())
		field->fieldValue.iValue = 0;
	return E_OK;
}

static _DBERROR AssField4Long( otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal)
{
	field->iType = FIELD_LONG;
	stmt>>field->fieldValue.lValue;
	if(stmt.is_null())
		field->fieldValue.lValue = 0;
	return E_OK;
}


static _DBERROR AssField4Timestamp( otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal)
{
	otl_datetime tmp;
	stmt>>tmp;
	field->iType = FIELD_STRING;
	if(stmt.is_null())
	{
		field->fieldValue.strValue = new char[1]();
		field->fieldValue.strValue[0] = '\0';
	}
	else
	{
		field->fieldValue.strValue = new char[14+1]();
		sprintf (field->fieldValue.strValue,"%04d%02d%02d%02d%02d%02d", tmp.year,tmp.month,tmp.day,tmp.hour,tmp.minute,tmp.second);
	}

	return E_OK;
}

std::map<int,AssFieldFunc> assFieldRel = {
		{otl_var_char,AssField4VarChar},
		{otl_var_double,AssField4Double},
		{otl_var_float,AssField4Float},
		{otl_var_int,AssField4Int},
		{otl_var_unsigned_int,AssField4Int},
		{otl_var_short,AssField4Int},
		{otl_var_long_int,AssField4Long},
		{otl_var_timestamp,AssField4Timestamp}
};


static _DBERROR BindInteger(otl_stream &stmt ,DbFieldBinder &binder)
{
	stmt<<binder.fieldValue.iValue;
	return E_OK;
}

static _DBERROR BindLong( otl_stream &stmt ,DbFieldBinder &binder)
{
	stmt<<binder.fieldValue.lValue;
	return E_OK;
}

static _DBERROR BindFloat( otl_stream &stmt ,DbFieldBinder &binder)
{
	stmt<<binder.fieldValue.fValue;
	return E_OK;
}

static _DBERROR BindDouble( otl_stream &stmt ,DbFieldBinder &binder)
{
	stmt<<binder.fieldValue.dValue;
	return E_OK;
}

static _DBERROR BindString( otl_stream &stmt ,DbFieldBinder &binder)
{
	stmt<<binder.fieldValue.strValue;
	return E_OK;
}

std::map<int , BindFunc>  Executor::bindParamRel = {
	{FIELD_INT, BindInteger},
	{FIELD_LONG, BindLong},
	{FIELD_FLOAT, BindFloat},
	{FIELD_DOUBLE, BindDouble},
	{FIELD_STRING, BindString}
};


Executor::Executor(otl_connect *conn)
:m_conn(conn)
{

}
Executor::~Executor()
{

}

std::shared_ptr<otl_stream> Executor::Query(std::string sql  , int buffSize )
{
	try
	{
		std::shared_ptr<otl_stream> ptr = std::make_shared<otl_stream>();
		ptr->open(buffSize , sql.c_str() , *m_conn);
		ptr->set_commit(0);  ///取消流的自动提交
		return ptr;
	}
	catch(otl_exception& e)
	{
		if(e.code == ORA_PIPE_BREAK || e.code == ORA_DISCONNECT)
		{
			THROW_C(DBConnBreakException,E_DISCONNECT,Errno2String(E_DISCONNECT));
		}
		throw;
	}
}

void Executor::FetchData(std::shared_ptr<otl_stream> stmt , _RESULT_ROW_VEC &result , int buffSize )
{
	try
	{
		otl_column_desc* desc;
		int desc_len=0;
		_RESULT_ROW record;

		result.clear();
		desc=stmt->describe_select(desc_len);
		while(!stmt->eof() && ( 0 < buffSize--) ) // while not end-of-data
		{
			DbFieldResult *dbField = NULL;
			for (int n = 0; n < desc_len;++n)
			{
				dbField = new DbFieldResult();
				assFieldRel[desc[n].otl_var_dbtype](*stmt , desc[n] , dbField, false);
				record.push_back(std::shared_ptr<DbFieldResult>(dbField));
			}
			result.push_back(record);
			record.clear();
		}
	}
	catch(otl_exception& e)
	{
		if(e.code == ORA_PIPE_BREAK || e.code == ORA_DISCONNECT)
		{
			THROW_C(DBConnBreakException,E_DISCONNECT,Errno2String(E_DISCONNECT));
		}
		throw;
	}
}


void Executor::FetchOrgData(std::shared_ptr<otl_stream> stmt , _RESULT_ROW_VEC &result , int buffSize )
{
	try
	{
		otl_column_desc* desc;
		int desc_len=0;
		_RESULT_ROW record;

		desc=stmt->describe_select(desc_len);
		while(!stmt->eof() && ( 0 < buffSize--) ) // while not end-of-data
		{
			DbFieldResult *dbField = NULL;
			for (int n = 0; n < desc_len;++n)
			{
				dbField = new DbFieldResult();
				assFieldRel[desc[n].otl_var_dbtype](*stmt , desc[n] , dbField,true);
				record.push_back(std::shared_ptr<DbFieldResult>(dbField));
			}
			result.push_back(record);
			record.clear();
		}
	}
	catch(otl_exception& e)
	{
		if(e.code == ORA_PIPE_BREAK || e.code == ORA_DISCONNECT)
		{
			THROW_C(DBConnBreakException,E_DISCONNECT,Errno2String(E_DISCONNECT));
		}
		throw;
	}
}


std::shared_ptr<otl_stream> Executor::Execute(std::string sql  , int buffSize )
{
	try
	{
		std::shared_ptr<otl_stream> ptr = std::make_shared<otl_stream>();
		ptr->open(buffSize , sql.c_str() , *m_conn);
		ptr->set_commit(0);  ///取消流的自动提交
		return ptr;
	}
	catch(otl_exception& e)
	{
		if(e.code == ORA_PIPE_BREAK || e.code == ORA_DISCONNECT)
		{
			THROW_C(DBConnBreakException,E_DISCONNECT,Errno2String(E_DISCONNECT));
		}
		throw;
	}
}


////单条记录绑定
void Executor::BindParam(std::shared_ptr<otl_stream> otl_stmt , _BINDER_VEC &paramVec , bool bAutoFlush)
{
	try
	{
		for(auto it= paramVec.begin(); it != paramVec.end(); ++it)
			bindParamRel[it->iType](*otl_stmt,*it);

		if(bAutoFlush)
			otl_stmt->flush(); /// 刷新绑定变量缓冲区，使sql真正执行；
	}
	catch(otl_exception& e)
	{
		if(e.code == ORA_PIPE_BREAK || e.code == ORA_DISCONNECT)
		{
			THROW_C(DBConnBreakException,E_DISCONNECT,Errno2String(E_DISCONNECT));
		}
		throw;
	}
}
//
//void Executor::BindParam(std::shared_ptr<otl_stream> otl_stmt,bool bAutoFlush,int paramCnt,... )
//{
//	try
//	{
//		// (1) 定义参数列表
//		va_list ap;
//		// (2) 初始化参数列表
//		va_start(ap, paramCnt);
//		// 获取参数值
//		DbFieldBinder binder;
//		while(paramCnt > 0)
//		{
//			binder = va_arg(ap, DbFieldBinder);
//			bindParamRel[binder.iType](*otl_stmt,binder);
//			--paramCnt;
//		}
//		// 关闭参数列表
//		va_end(ap);
//
//		if(bAutoFlush)
//			otl_stmt->flush(); /// 刷新绑定变量缓冲区，使sql真正执行；
//	}
//	catch(otl_exception& e)
//	{
//		if(e.code == ORA_PIPE_BREAK || e.code == ORA_DISCONNECT)
//		{
//			THROW_C(DBConnBreakException,E_DISCONNECT,Errno2String(E_DISCONNECT));
//		}
//		throw;
//	}
//}
void Executor::BatBindParam(std::shared_ptr<otl_stream> otl_stmt , std::vector<_BINDER_VEC> &mutiParamVec ,std::vector<size_t> *errVec)
{
	try
	{
		for(auto it  = mutiParamVec.begin() ;it != mutiParamVec.end(); ++it)
			BindParam(otl_stmt , *it , false);

		///正常缓冲区满后自动刷新
		otl_stmt->flush();/// 刷新绑定变量缓冲区，使sql真正执行；
	}
	catch(otl_exception& e)
	{
		if(e.code == ORA_PIPE_BREAK || e.code == ORA_DISCONNECT)
		{
			THROW_C(DBConnBreakException,E_DISCONNECT,Errno2String(E_DISCONNECT));
		}
#ifndef OTL_ODBC
		long rpc = 0;  ///成功执行记录数
		long total_rpc = 0;   ///总的执行数
		bool bGoOn = false;
		do{
			try
			{
				////flush后重新计数
				rpc=otl_stmt->get_rpc();
				total_rpc += rpc;
				_TRACE_MSG("TOTAL_RPC= %ld,RPC= %ld \n" ,total_rpc ,rpc);
				////20150608
				if(NULL != errVec)
					errVec->push_back(total_rpc);   ////出错行位置号
				//bypass the erro row and start
				total_rpc += 1;
				otl_stmt->flush(total_rpc,true);
				bGoOn = false;
			}
			catch(otl_exception& e1)
			{
					bGoOn = true;
			}
		}while(bGoOn);
#endif
	}

}

void Executor::Commit(otl_connect *conn)
{
	try
	{
		conn->commit();
	}
	catch(otl_exception& e)
	{
		if(e.code == ORA_PIPE_BREAK || e.code == ORA_DISCONNECT)
		{
			THROW_C(DBConnBreakException,E_DISCONNECT,Errno2String(E_DISCONNECT));
		}
		THROW_C_P3(DBSqlException , E_STMT_EXEC,Errno2String(E_STMT_EXEC),e.msg,e.stm_text,e.var_info);
	}
}

void Executor::Rollback(otl_connect *conn)
{
	try
	{
		conn->rollback();
	}
	catch(otl_exception& e)
	{
		if(e.code == ORA_PIPE_BREAK || e.code == ORA_DISCONNECT)
		{
			THROW_C(DBConnBreakException,E_DISCONNECT,Errno2String(E_DISCONNECT));
		}
		THROW_C_P3(DBSqlException , E_STMT_EXEC,Errno2String(E_STMT_EXEC),e.msg,e.stm_text,e.var_info);
	}

}
}



