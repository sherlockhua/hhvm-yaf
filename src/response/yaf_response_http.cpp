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

namespace HPHP {

int yaf_response_set_redirect(Variant response, const String& location)
{
    Transport *transport = g_context->getTransport();
    if (transport) {
        transport->replaceHeader("Location", location.toCppString().c_str());
    }

    return 0;
}

}
