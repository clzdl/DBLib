/*
 * BilErrCode.h
 *
 *  Created on: 2015年1月20日
 *      Author: chengl
 */

#ifndef _ERR_CODE_H
#define _ERR_CODE_H
#include "Exception.h"
#include "MacroDefUtil.h"
namespace DBLib{

////转换错误码定义
#define _ERR_MAP(X)                                            \
    X(OK, "0000" , "成功")                                     \
    X(FAIL,"0001" , "操作失败")                             	  \
	X(DISCONNECT,"0002" , "连接断开")                           \
	X(BIND_PARAM_CNT,"0003" , "参数绑定个数不匹配")                           \
	X(BIND_PARAM,"0004" , "参数绑定失败")                           \
	X(META_DATA,"0005" , "获取元数据失败")                           \
	X(BIND_RESULT,"0006" , "获取结果缓冲区失败")                           \
	X(STMT_EXEC,"0007" , "预解析语句执行失败")                           \
	X(NO_DATA,"0008" , "无数据")                           \
	X(DB_ERROR,"0009" , "数据库错误")                           \


/* Define TRE_* values for each errno value above */
#define  _ERR_GEN(n, c , s) E_##n,

typedef enum
{
	_ERR_MAP(_ERR_GEN)
} _DBERROR;

#undef _ERRNO_GEN


#define _DUMP_EXCEPTION(e)	fprintf(stderr,"msg:%s,stm_text:%s,var_info:%s\n",e.msg,e.stm_text,e.var_info)

///////oracle error
#define ORA_PIPE_BREAK       3113
#define ORA_DISCONNECT       3114

using namespace CommonUtils;
/**
 * 定义Oracle数据库异常
 */
DECLARE_EXCEPTION(DBException, CommonUtils::Exception);
///oracle 数据库连接断开
DECLARE_EXCEPTION(DBConnBreakException, DBException);
///oracle 数据库sql错误
DECLARE_EXCEPTION(DBSqlException, DBException);



}
#endif /* _ERR_CODE_H */
