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
#include "ext_yaf.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/std/ext_std_classobj.h"
#include "hphp/runtime/ext/extension.h"

#include <stdio.h>
#include <stdarg.h>


namespace HPHP {
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


static void yaf_throw_exception(int level, const char* msg)
{
    Object o = Object(SystemLib::AllocExceptionObject(msg));
    if ((level & YAF_ERR_BASE) == YAF_ERR_BASE) {

        Array params = Array::Create();
        params.append(String(msg));
        params.append(Variant(level));

        switch (level) {
            case YAF_ERR_STARTUP_FAILED:
                o = createObject("Yaf_Exception_StartupError", params);
                break;
            case YAF_ERR_ROUTE_FAILED:
                o = createObject("Yaf_Exception_RouterFailed", params);
                break;
            case YAF_ERR_DISPATCH_FAILED:
                o = createObject("Yaf_Exception_DispatchFailed", params);
                break;
            case YAF_ERR_NOTFOUND_MODULE:
                o = createObject("Yaf_Exception_LoadFailed_Module", params);
                break;
            case YAF_ERR_NOTFOUND_CONTROLLER:
                o = createObject("Yaf_Exception_LoadFailed_Controller", params);
                break;
            case YAF_ERR_NOTFOUND_ACTION:
                o = createObject("Yaf_Exception_LoadFailed_Action", params);
                break;
            case YAF_ERR_NOTFOUND_VIEW:
                o = createObject("Yaf_Exception_LoadFailed_View", params);
                break;
            case YAF_ERR_AUTOLOAD_FAILED:
                o = createObject("Yaf_Exception_LoadFailed", params);
                break;
            case YAF_ERR_TYPE_ERROR:
                o = createObject("Yaf_Exception_TypeError", params);
                break;
        }
    }

    throw o;
}

void yaf_trigger_error(int level, char* format, ...)
{
    char msg_buf[8192];
    va_list va;
    va_start(va, format);
    int ret = vsnprintf(msg_buf, sizeof(msg_buf), format, va);            
    va_end(va); 

    if (g_yaf_local_data.get()->throw_exception) {
        yaf_throw_exception(level, msg_buf);
    } else {
        raise_error("%s", msg_buf);
    }
}

static void HHVM_METHOD(Yaf_Exception, __construct,
        const Variant& message, const Variant& code,
        const Variant& previous)
{
    if (!message.isNull()) {
        auto ptr_message = this_->o_realProp("message",
                            ObjectData::RealPropUnchecked, "Yaf_Exception");
        *ptr_message = message;
    }

    if (!code.isNull()) {
        auto ptr_code = this_->o_realProp("code",
                            ObjectData::RealPropUnchecked, "Yaf_Exception");
        *ptr_code = code;
    }

    if (!previous.isNull()) {
        auto ptr_previous = this_->o_realProp("previous",
                            ObjectData::RealPropUnchecked, "Yaf_Exception");
        *ptr_previous = previous;
    }
}

/*
static Variant HHVM_METHOD(Yaf_Exception, getPrevious)
{
    auto ptr_previous = this_->o_realProp("previous",
                        ObjectData::RealPropUnchecked, "Yaf_Exception");
    return *ptr_previous;
}
*/

void YafExtension::_initYafExceptionClass()
{
    HHVM_ME(Yaf_Exception, __construct);
    //HHVM_ME(Yaf_Exception, getPrevious);
}



}
