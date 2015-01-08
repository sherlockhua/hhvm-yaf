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
#include "yaf_request_simple.h"
#include "ext_yaf.h"
#include "hphp/runtime/base/base-includes.h"


namespace HPHP{

void YafExtension::_initYafRequestSimpleClass()
{
    HHVM_ME(Yaf_Request_Simple, getQuery);
    HHVM_ME(Yaf_Request_Simple, getRequest);
    HHVM_ME(Yaf_Request_Simple, getPost);
    HHVM_ME(Yaf_Request_Simple, getCookie);
    HHVM_ME(Yaf_Request_Simple, getFiles);
    HHVM_ME(Yaf_Request_Simple, get);
    HHVM_ME(Yaf_Request_Simple, isXmlHttpRequest);
    HHVM_ME(Yaf_Request_Simple, __construct);
    HHVM_ME(Yaf_Request_Simple, __clone);

}

}
