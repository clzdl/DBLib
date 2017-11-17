/*
 * Executor.cpp
 *
 *  Created on: 2017年9月15日
 *      Author: cplusplus
 */


#include "Executor.h"

namespace DBLib{

//获取结果集的具体字段
typedef void (*AssFieldFunc)( otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal);

static void AssField4VarChar(otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal)
{
	field->fieldValue.strValue = new char[desc.dbsize+1]();
	field->iType = FIELD_STRING;
	stmt>>field->fieldValue.strValue;
}

static void AssField4Double( otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal)
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

}


static void AssField4Long( otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal)
{
	field->iType = FIELD_LONG;
	stmt>>field->fieldValue.lValue;
	if(stmt.is_null())
		field->fieldValue.lValue = 0;
}


static void AssField4Timestamp( otl_stream &stmt,otl_column_desc &desc,DbFieldResult *field,bool isOriginal)
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
}

std::map<int,AssFieldFunc> assFieldRel = {
		{otl_var_char,AssField4VarChar},
		{otl_var_char,AssField4VarChar},
		{otl_var_bigint,AssField4Long},
		{otl_var_short,AssField4Long},
		{otl_var_unsigned_int,AssField4Long},
		{otl_var_int,AssField4Long},
		{otl_var_long_int,AssField4Long},
		{otl_var_float,AssField4Double},
		{otl_var_double,AssField4Double},
		{otl_var_timestamp,AssField4Timestamp}
};


static void BindLong( otl_stream &stmt ,DbFieldBinder &binder)
{
	stmt<<binder.fieldValue.lValue;
}

static void BindDouble( otl_stream &stmt ,DbFieldBinder &binder)
{
	stmt<<binder.fieldValue.dValue;
}

static void BindString( otl_stream &stmt ,DbFieldBinder &binder)
{
	stmt<<binder.fieldValue.strValue;
}

std::map<int , BindFunc>  Executor::bindParamRel = {
	{FIELD_LONG, BindLong},
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
			THROW(DBConnBreakException,ORA_BRK_EXP_MSG);
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
			THROW(DBConnBreakException,ORA_BRK_EXP_MSG);
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
			THROW(DBConnBreakException,ORA_BRK_EXP_MSG);
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
			THROW(DBConnBreakException,ORA_BRK_EXP_MSG);
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
			THROW(DBConnBreakException,ORA_BRK_EXP_MSG);
		}
		throw;
	}
}
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
			THROW(DBConnBreakException,ORA_BRK_EXP_MSG);
		}
#ifndef OTL_ODBC
		if(nullptr == errVec)
		{
			throw;
		}
		else
		{
			long rpc = 0;  ///成功执行记录数
			long total_rpc = 0;   ///总的执行数
			bool bGoOn = false;
			do{
				try
				{
					////flush后重新计数
					rpc=otl_stmt->get_rpc();
					total_rpc += rpc;
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
		}
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
			THROW(DBConnBreakException,ORA_BRK_EXP_MSG);
		}
		throw;
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
			THROW(DBConnBreakException,ORA_BRK_EXP_MSG);
		}
		throw;
	}

}


void Executor::Commit()
{
	try
	{
		m_conn->commit();
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

void Executor::Rollback()
{
	try
	{
		m_conn->rollback();
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
}



