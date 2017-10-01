/*
 * DbFieldBinder.cpp
 *
 *  Created on: 2017年9月25日
 *      Author: cplusplus
 */

#include "DbFieldBinder.h"

namespace DBLib{

DbFieldBinder::DbFieldBinder()
:iType(FIELD_INT)
{

}
DbFieldBinder::DbFieldBinder(_FIELD_TYPE type)
:iType(type)
{

}

DbFieldBinder::DbFieldBinder(long value)
:iType(FIELD_LONG)
{
	fieldValue.lValue = value;
}
DbFieldBinder::DbFieldBinder(double value)
:iType(FIELD_DOUBLE)
{
	fieldValue.dValue = value;
}
DbFieldBinder::DbFieldBinder(char* value)
:iType(FIELD_STRING)
{
	fieldValue.strValue = value;
}

DbFieldBinder::~DbFieldBinder()
{

}

void DbFieldBinderHelper::FreeSingleParamVec( std::vector<DbFieldBinder> &vec)
{
	for(auto it = vec.begin() ; it != vec.end(); ++it)
	{
		if(it->iType == FIELD_STRING)
		{
			if(NULL != it->fieldValue.strValue )
			{
				delete [] it->fieldValue.strValue;
				it->fieldValue.strValue = NULL;
			}
		}
	}
}
void DbFieldBinderHelper::FreeMultipleParamVec( std::vector< std::vector<DbFieldBinder> > &vec)
{
	for(auto it = vec.begin() ; it != vec.end() ; ++it)
	{
		FreeSingleParamVec(*it);
	}
}

void DbFieldBinderHelper::BuildBinder4Int(_BINDER_VEC &vecParams,int value)
{
	DbFieldBinder binder(DBLib::FIELD_INT);
	binder.fieldValue.iValue = value;
	vecParams.push_back(binder);
}
void DbFieldBinderHelper::BuildBinder4Long(_BINDER_VEC &vecParams,long value)
{
	DbFieldBinder binder(DBLib::FIELD_LONG);
	binder.fieldValue.lValue = value;
	vecParams.push_back(binder);
}
void DbFieldBinderHelper::BuildBinder4Float(_BINDER_VEC &vecParams,float value)
{
	DbFieldBinder binder(DBLib::FIELD_FLOAT);
	binder.fieldValue.fValue = value;
	vecParams.push_back(binder);
}
void DbFieldBinderHelper::BuildBinder4Double(_BINDER_VEC &vecParams,double value)
{
	DbFieldBinder binder(DBLib::FIELD_DOUBLE);
	binder.fieldValue.dValue = value;
	vecParams.push_back(binder);
}
void DbFieldBinderHelper::BuildBinder4String(_BINDER_VEC &vecParams,char* value)
{
	DbFieldBinder binder(DBLib::FIELD_STRING);
	binder.fieldValue.strValue = value;
	vecParams.push_back(binder);
}

}

