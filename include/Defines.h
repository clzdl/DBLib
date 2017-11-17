/*
 * Def.h
 *
 *  Created on: 2017年9月25日
 *      Author: cplusplus
 */

#ifndef INCLUDE_DEFINES_H_
#define INCLUDE_DEFINES_H_
#include "Exception.h"

namespace DBLib{

typedef enum
{
	FIELD_LONG,
	FIELD_DOUBLE,
	FIELD_STRING,
}_FIELD_TYPE;

typedef union
{
	long lValue;
	double dValue;
	char *strValue;
} _FIELD_VALUE;



#define _DUMP_EXCEPTION(e)	fprintf(stderr,"msg:%s,stm_text:%s,var_info:%s\n",e.msg,e.stm_text,e.var_info)

///////oracle error
#define ORA_PIPE_BREAK       3113
#define ORA_DISCONNECT       3114


#define ORA_BRK_EXP_MSG 	"数据库连接已断开"
/**
 * 定义Oracle数据库异常
 */
///oracle 数据库连接断开
DECLARE_EXCEPTION(DBConnBreakException, CommonUtils::Exception);

}



#endif /* INCLUDE_DEFINES_H_ */
