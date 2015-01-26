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

void yaf_trigger_error(int level, char* format, ...)
{
    printf("err:%s", format);
}
