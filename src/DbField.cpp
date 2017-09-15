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

}


