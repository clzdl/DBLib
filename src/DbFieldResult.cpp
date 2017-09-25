/*
 * DbField.cpp
 *
 *  Created on: 2017年9月5日
 *      Author: cplusplus
 */

#include <DbFieldResult.h>
#include "stdlib.h"
#include "string.h"


namespace DBLIB{

DbFieldResult::DbFieldResult()
{

}

DbFieldResult::DbFieldResult(_FIELD_TYPE type)
:iType(type)
{
}

DbFieldResult::~DbFieldResult()
{
	if(iType == DBLIB::FIELD_STRING && fieldValue.strValue != NULL)
	{
		delete [] fieldValue.strValue;
		fieldValue.strValue = NULL;
	}
}

DbFieldResult::DbFieldResult(const DbFieldResult &field)
:iType(field.iType)
{
	if(FIELD_STRING == iType)
	{
		fieldValue.strValue = new char[strlen(field.fieldValue.strValue+1)]();
		strcpy(fieldValue.strValue,field.fieldValue.strValue);
	}else{
		memcpy(&fieldValue, &field.fieldValue , sizeof(field.fieldValue));
	}
}
DbFieldResult& DbFieldResult::operator = (const DbFieldResult &field)
{
	if(this == &field)
		return *this;

	iType = field.iType;

	if(FIELD_STRING == iType)
	{
		fieldValue.strValue = new char[strlen(field.fieldValue.strValue+1)]();
		strcpy(fieldValue.strValue,field.fieldValue.strValue);
	}else{
		memcpy(&fieldValue, &field.fieldValue , sizeof(field.fieldValue));
	}

	return *this;
}

}


