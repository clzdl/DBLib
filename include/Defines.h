/*
 * Def.h
 *
 *  Created on: 2017年9月25日
 *      Author: cplusplus
 */

#ifndef INCLUDE_DEFINES_H_
#define INCLUDE_DEFINES_H_

namespace DBLIB{

typedef enum
{
	FIELD_INT = 0,
	FIELD_LONG,
	FIELD_FLOAT,
	FIELD_DOUBLE,
	FIELD_STRING,
}_FIELD_TYPE;

typedef union
{
	int iValue;
	long lValue;
	float fValue;
	double dValue;
	char *strValue;
} _FIELD_VALUE;

}



#endif /* INCLUDE_DEFINES_H_ */
