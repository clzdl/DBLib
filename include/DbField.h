/*
 * DbField.h
 *
 *  Created on: 2017年9月5日
 *      Author: cplusplus
 */

#ifndef INCLUDE_DBFIELD_H_
#define INCLUDE_DBFIELD_H_

#include <vector>
#include <memory>

namespace DBLIB{

typedef enum _FIELD_TYPE
{
	FIELD_INT = 0,
	FIELD_LONG,
	FIELD_FLOAT,
	FIELD_DOUBLE,
	FIELD_STRING,
}_FIELD_TYPE;

typedef union _FieldValue
{
	int iValue;
	long lValue;
	float fValue;
	double dValue;
	char *strValue;
}FieldValue;

class DbField
{
public:
	DbField();
	DbField(_FIELD_TYPE type);
	~DbField();
	DbField(const DbField &field);
	DbField& operator = (const DbField &field);

	_FIELD_TYPE iType;
	_FieldValue fieldValue;
};

//提取结果集使用
typedef std::vector< std::shared_ptr<DbField> >  _ROW ;
//绑定参数使用
typedef std::vector<DbField> _PARAM_VEC;

}



#endif /* INCLUDE_DBFIELD_H_ */
