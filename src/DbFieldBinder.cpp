/*
 * DbFieldBinder.cpp
 *
 *  Created on: 2017年9月25日
 *      Author: cplusplus
 */

#include "DbFieldBinder.h"
#include "string.h"

namespace DBLib{

DbFieldBinder::DbFieldBinder()
:iType(FIELD_LONG)
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
void DbFieldBinderHelper::BuildBinder4Long(_BINDER_VEC &vecParams,long value)
{
	DbFieldBinder binder(DBLib::FIELD_LONG);
	binder.fieldValue.lValue = value;
	vecParams.push_back(binder);
}
void DbFieldBinderHelper::BuildBinder4Double(_BINDER_VEC &vecParams,double value)
{
	DbFieldBinder binder(DBLib::FIELD_DOUBLE);
	binder.fieldValue.dValue = value;
	vecParams.push_back(binder);
}
void DbFieldBinderHelper::BuildBinder4String(_BINDER_VEC &vecParams,const char* value)
{
	DbFieldBinder binder(DBLib::FIELD_STRING);
	binder.fieldValue.strValue = const_cast<char*>(value);
	vecParams.push_back(binder);
}
void DbFieldBinderHelper::BuildBinder4String(_BINDER_VEC &vecParams,std::string &value)
{
	BuildBinder4String(vecParams,const_cast<char*>(value.c_str()));
}

void DbFieldBinderHelper::BuildDynamicBinder4String(_BINDER_VEC &vecParams,std::string value)
{
	DbFieldBinder binder(DBLib::FIELD_STRING);
	binder.fieldValue.strValue = new char[value.size() + 1];
	strcpy(binder.fieldValue.strValue,value.c_str());
	vecParams.push_back(binder);
}
void DbFieldBinderHelper::BuildDynamicBinder4String(_BINDER_VEC &vecParams,const char* value)
{
	DbFieldBinder binder(DBLib::FIELD_STRING);
	binder.fieldValue.strValue = new char[strlen(value)+1];
	strcpy(binder.fieldValue.strValue,value);
	vecParams.push_back(binder);
}
void DbFieldBinderHelper::BuildDynamicBinderFree(_BINDER_VEC &vecParams)
{
	for(auto &it : vecParams)
	{
		if(it.iType == FIELD_STRING && nullptr != it.fieldValue.strValue)
		{
			delete [] it.fieldValue.strValue;
			it.fieldValue.strValue = nullptr;
		}
	}
}

void DbFieldBinderHelper::BuildDynamicMutiBinderFree(std::vector<_BINDER_VEC> &vecParams)
{
	for(auto &it : vecParams)
	{
		BuildDynamicBinderFree(it);
	}
}

std::stringstream DbFieldBinderHelper::PrintBinderCache(_BINDER_VEC &vecParams)
{
	std::stringstream ss;
	for(DbFieldBinder binder : vecParams)
	{
		switch(binder.iType){
		case FIELD_LONG:
			ss<<binder.fieldValue.lValue<<",";
			break;
		case FIELD_DOUBLE:
			ss<<binder.fieldValue.dValue<<",";
			break;
		case FIELD_STRING:
			ss<<binder.fieldValue.strValue<<",";
			break;
		}
	}
	return ss;
}

}

