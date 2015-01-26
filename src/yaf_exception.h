/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-23 15:48:03
*   description  ：
*
=============================================*/
#ifndef _YAF_EXCEPTION_H_
#define _YAF_EXCEPTION_H_

#define YAF_MAX_BUILDIN_EXCEPTION   10

#define YAF_ERR_BASE                512
#define YAF_UERR_BASE               1024
#define YAF_ERR_MASK                127

#define YAF_ERR_STARTUP_FAILED      512
#define YAF_ERR_ROUTE_FAILED        513
#define YAF_ERR_DISPATCH_FAILED     514
#define YAF_ERR_NOTFOUND_MODULE     515
#define YAF_ERR_NOTFOUND_CONTROLLER 516
#define YAF_ERR_NOTFOUND_ACTION     517
#define YAF_ERR_NOTFOUND_VIEW       518
#define YAF_ERR_CALL_FAILED         519
#define YAF_ERR_AUTOLOAD_FAILED     520
#define YAF_ERR_TYPE_ERROR          521
#define YAF_ERR_TYPE_WARN          522


extern void yaf_trigger_error(int type, char* format, ...);



#endif //_YAF_EXCEPTION_H
