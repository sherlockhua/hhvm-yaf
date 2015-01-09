/*===========================================
*   Copyright (C) 2014 All rights reserved.
*   
*   company      : xiaomi
*   author       : root
*   email        : root@xiaomi.com
*   date         ：2014-12-18 07:29:18
*   description  ：
*
=============================================*/
#include "yaf_response.h"
#include "ext_yaf.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/base/php-globals.h"


namespace HPHP{

static void HHVM_METHOD(Yaf_Response_Abstract, __clone)
{
}

static void HHVM_METHOD(Yaf_Response_Abstract, __construct)
{

}

void YafExtension::_initYafRequestClass()
{
    HHVM_ME(Yaf_Response_Abstract, __clone);
    HHVM_ME(Yaf_Response_Abstract, __construct);

    _initYafResponseHttpClass();
    _initYafResponseClientClass();
}

}
