/*
 * DbField.cpp
 *
 *  Created on: 2017年9月5日
 *      Author: cplusplus
 */

#include "DbField.h"
#include "stdlib.h"
#include "string.h"


namespace DBLIB{

DbField::DbField()
{

}

DbField::DbField(_FIELD_TYPE type)
:iType(type)
{
}

DbField::~DbField()
{
	if(iType == DBLIB::FIELD_STRING && fieldValue.strValue != NULL)
	{
		delete [] fieldValue.strValue;
		fieldValue.strValue = NULL;
	}
}

DbField::DbField(const DbField &field)
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
DbField& DbField::operator = (const DbField &field)
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


