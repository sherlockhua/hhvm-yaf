/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-12 10:29:15
*   description  ：
*
=============================================*/
#include "yaf_application.h"
#include "ext_yaf.h"


namespace HPHP{

static void HHVM_METHOD(Yaf_Application, __clone)
{
}

static void HHVM_METHOD(Yaf_Application, __construct)
{
}

void YafExtension::_initYafApplicationClass()
{
    HHVM_ME(Yaf_Application, __clone);
    HHVM_ME(Yaf_Application, __construct);
}

}
