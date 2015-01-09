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

int yaf_response_set_redirect(Variant response, const String& location)
{
    Transport *transport = g_context->getTransport();
    if (transport) {
        transport->replaceHeader("Location", location.toCppString().c_str());
    }

    return 0;
}

static void HHVM_METHOD(Yaf_Response_Http, __clone)
{
}

static void HHVM_METHOD(Yaf_Response_Http, __construct)
{

}



void YafExtension::_initYafResponseHttpClass()
{
    HHVM_ME(Yaf_Response_Http, __construct);
    HHVM_ME(Yaf_Response_Http, __clone);
}

}
