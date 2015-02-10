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
#include "ext_yaf.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/base/php-globals.h"
#include "requests/yaf_request_http.h"

namespace HPHP{


static Variant HHVM_METHOD(Yaf_Plugin_Abstract, routerStartup, 
        const Variant& request, const Variant& response) 
{
    return true;
}

static Variant HHVM_METHOD(Yaf_Plugin_Abstract, routerShutdown, 
        const Variant& request, const Variant& response) 
{
    return true;
}

static Variant HHVM_METHOD(Yaf_Plugin_Abstract, dispatchLoopStartup, 
        const Variant& request, const Variant& response) 
{
    return true;
}

static Variant HHVM_METHOD(Yaf_Plugin_Abstract, dispatchLoopShutdown, 
        const Variant& request, const Variant& response) 
{
    return true;
}

static Variant HHVM_METHOD(Yaf_Plugin_Abstract, preDispatch, 
        const Variant& request, const Variant& response) 
{
    return true;
}

static Variant HHVM_METHOD(Yaf_Plugin_Abstract, postDispatch, 
        const Variant& request, const Variant& response) 
{
    return true;
}

static Variant HHVM_METHOD(Yaf_Plugin_Abstract, preResponse, 
        const Variant& request, const Variant& response) 
{
    return true;
}

void YafExtension::_initYafPluginClass()
{
    HHVM_ME(Yaf_Plugin_Abstract, routerStartup);
    HHVM_ME(Yaf_Plugin_Abstract, routerShutdown);
    HHVM_ME(Yaf_Plugin_Abstract, dispatchLoopStartup);
    HHVM_ME(Yaf_Plugin_Abstract, dispatchLoopShutdown);
    HHVM_ME(Yaf_Plugin_Abstract, preDispatch);
    HHVM_ME(Yaf_Plugin_Abstract, postDispatch);
    HHVM_ME(Yaf_Plugin_Abstract, preResponse);
}

}
