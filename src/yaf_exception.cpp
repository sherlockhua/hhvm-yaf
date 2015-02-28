/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-23 15:54:11
*   description  ：
*
=============================================*/
#include "yaf_exception.h"

#include <stdio.h>
#include <stdarg.h>


void yaf_trigger_error(int level, char* format, ...)
{
    char msg_buf[8192];
    va_list va;
    va_start(va, format);
    int ret = vsnprintf(msg_buf, sizeof(msg_buf), format, va);            
    va_end(va); 

    printf("error:%s\n", msg_buf);
}

