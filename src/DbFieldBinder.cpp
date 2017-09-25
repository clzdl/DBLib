/*
 * DbFieldBinder.cpp
 *
 *  Created on: 2017年9月25日
 *      Author: cplusplus
 */

#include "DbFieldBinder.h"



namespace DBLIB{
DbFieldBinder::DbFieldBinder()
{

}
DbFieldBinder::~DbFieldBinder()
{

}

void DbFieldBinder::FreeSingleParamVec( std::vector<DbFieldBinder> &vec)
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
void DbFieldBinder::FreeMultipleParamVec( std::vector< std::vector<DbFieldBinder> > &vec)
{
	for(auto it = vec.begin() ; it != vec.end() ; ++it)
	{
		FreeSingleParamVec(*it);
	}
}

}

