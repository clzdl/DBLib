/*
 * Executor.cpp
 *
 *  Created on: 2017年9月15日
 *      Author: cplusplus
 */


#include "Executor.h"
#include <map>

namespace DBLIB{

typedef _DBERROR (*AssFieldFunc)( otl_stream &stmt,otl_column_desc &desc,DbField *field);


_DBERROR AssField4VarChar(otl_stream &stmt,otl_column_desc &desc,DbField *field)
{
	field->fieldValue.strValue = new char[desc.dbsize+1]();
	field->iType = FIELD_STRING;
	stmt>>field->fieldValue.strValue;
	return E_OK;
}

_DBERROR AssField4Double( otl_stream &stmt,otl_column_desc &desc,DbField *field)
{
	if(desc.scale == 0 )
	{
		field->iType = FIELD_INT;
		stmt>>field->fieldValue.lValue;
	}
	else
	{
		field->iType = FIELD_DOUBLE;
		stmt>>field->fieldValue.dValue;
	}
	if(stmt.is_null())
		field->fieldValue.dValue = 0.0;

	return E_OK;
}

_DBERROR AssField4Int( otl_stream &stmt,otl_column_desc &desc,DbField *field)
{
	field->iType = FIELD_INT;
	stmt>>field->fieldValue.iValue;
	if(stmt.is_null())
		field->fieldValue.iValue = 0;
	return E_OK;
}

_DBERROR AssField4Long( otl_stream &stmt,otl_column_desc &desc,DbField *field)
{
	field->iType = FIELD_LONG;
	stmt>>field->fieldValue.lValue;
	if(stmt.is_null())
		field->fieldValue.lValue = 0;
	return E_OK;
}


_DBERROR AssField4Timestamp( otl_stream &stmt,otl_column_desc &desc,DbField *field)
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
		{otl_var_float,AssField4Double},
		{otl_var_int,AssField4Int},
		{otl_var_unsigned_int,AssField4Int},
		{otl_var_short,AssField4Int},
		{otl_var_long_int,AssField4Long},
		{otl_var_timestamp,AssField4Timestamp}
};


Executor::Executor(otl_connect *conn)
:m_conn(conn)
{

}
Executor::~Executor()
{

}

otl_stream* Executor::Query(std::string sql  , int buffSize )
{
	otl_stream *stmtPtr = new otl_stream();
	stmtPtr->open(buffSize , sql.c_str() , *m_conn);
	return stmtPtr;
}

_DBERROR Executor::FetchData(otl_stream* stmt , _ROW_VEC &result , int buffSize )
{
	otl_column_desc* desc;
	int desc_len=0;
	_ROW record;
	desc=stmt->describe_select(desc_len);
	while(!stmt->eof() && ( 0 < buffSize--) ) // while not end-of-data
	{
		DbField *dbField = new DbField();
		for (int n = 0; n < desc_len;++n)
		{
			assFieldRel[desc[n].otl_var_dbtype](*stmt , desc[n] , dbField);
			record.push_back(std::shared_ptr<DbField>(dbField));
		}
		result.push_back(record);
		record.clear();
	}
	return E_OK;
}

}

