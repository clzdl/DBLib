/*
 * DbVarBinder.h
 *
 *  Created on: 2017年9月25日
 *      Author: cplusplus
 */

#ifndef INCLUDE_DBFIELDBINDER_H_
#define INCLUDE_DBFIELDBINDER_H_

#include <vector>
#include <memory>
#include "Defines.h"

namespace DBLIB{

/**
 *  sql 解析语句，参数绑定
 */
class DbFieldBinder
{
public:
	DbFieldBinder();
	~DbFieldBinder();

	/**
	 *  释放动态分配的字符串数组
	 */
	static void FreeSingleParamVec( std::vector<DbFieldBinder> &vec);

	/**
	 *  释放动态分配的字符串数组
	 */
	static void FreeMultipleParamVec( std::vector< std::vector<DbFieldBinder> > &vec);

	_FIELD_TYPE iType;
	_FIELD_VALUE fieldValue;
};


//绑定参数使用
typedef std::vector<DbFieldBinder> _BINDER_VEC;
}


#endif /* INCLUDE_DBFIELDBINDER_H_ */
