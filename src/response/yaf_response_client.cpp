/*===========================================
*   Copyright (C) 2014 All rights reserved.
*   
*   company      : xiaomi
*   author       : root
*   email        : root@xiaomi.com
*   date         ：2014-12-18 07:52:05
*   description  ：
*
=============================================*/
#include "yaf_response.h"
#include "ext_yaf.h"

namespace HPHP {

static void HHVM_METHOD(Yaf_Response_Cli, __clone)
{
}

static void HHVM_METHOD(Yaf_Response_Cli, __construct)
{

}



void YafExtension::_initYafResponseClientClass()
{
    HHVM_ME(Yaf_Response_Cli, __construct);
    HHVM_ME(Yaf_Response_Cli, __clone);
}



}
