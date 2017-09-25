/*
 * DbField.h
 *
 *  Created on: 2017年9月5日
 *      Author: cplusplus
 */

#ifndef INCLUDE_DBFIELDRESULT_H_
#define INCLUDE_DBFIELDRESULT_H_

#include <vector>
#include <memory>
#include "Defines.h"

namespace DBLib{

/**
 *  结果集提取
 */
class DbFieldResult
{
public:
	DbFieldResult();
	DbFieldResult(_FIELD_TYPE type);
	~DbFieldResult();
	DbFieldResult(const DbFieldResult &field);
	DbFieldResult& operator = (const DbFieldResult &field);

	_FIELD_TYPE iType;
	_FIELD_VALUE fieldValue;
};

//提取结果集使用
typedef std::vector< std::shared_ptr<DbFieldResult> >  _RESULT_ROW ;

typedef	std::vector<_RESULT_ROW>  _RESULT_ROW_VEC ;
}



#endif /* INCLUDE_DBFIELDRESULT_H_ */
