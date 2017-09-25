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

namespace DBLib{

/**
 *  sql 解析语句，参数绑定
 */
class DbFieldBinder
{
public:
	DbFieldBinder();
	DbFieldBinder(_FIELD_TYPE type);
	~DbFieldBinder();

	_FIELD_TYPE iType;
	_FIELD_VALUE fieldValue;
};

//绑定参数使用
typedef std::vector<DbFieldBinder> _BINDER_VEC;


//绑定参数辅助类
class DbFieldBinderHelper
{
public:
	/**
	 *  释放动态分配的字符串数组
	 */
	static void FreeSingleParamVec( std::vector<DbFieldBinder> &vec);
	/**
	 *  释放动态分配的字符串数组
	 */
	static void FreeMultipleParamVec( std::vector< std::vector<DbFieldBinder> > &vec);

	static void BuildBinder4Int(_BINDER_VEC &vecParams,int value);
	static void BuildBinder4Long(_BINDER_VEC &vecParams,long value);
	static void BuildBinder4Float(_BINDER_VEC &vecParams,float value);
	static void BuildBinder4Double(_BINDER_VEC &vecParams,double value);
	static void BuildBinder4String(_BINDER_VEC &vecParams,char* value);
};



}


#endif /* INCLUDE_DBFIELDBINDER_H_ */
