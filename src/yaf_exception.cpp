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

void yaf_trigger_error(int level, char* format, ...)
{
    char msg_buf[8192];
    va_list va;
    va_start(va, format);
    int ret = vsnprintf(msg_buf, sizeof(msg_buf), format, va);            
    va_end(va); 

    printf("error:%s\n", msg_buf);
    raise_warning("%s", msg_buf);
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
