/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-04-07 14:13:55
*   description  ：
*
=============================================*/
#ifndef _YAF_COMMON_H_
#define _YAF_COMMON_H_

#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/base/request-event-handler.h"
#include "hphp/runtime/base/request-local.h"
#include "hphp/runtime/base/builtin-functions.h"
#include "hphp/system/constants.h"

#if (HHVM_VERSION_MAJOR == 3) && (HHVM_VERSION_MINOR >= 5) 
#include "hphp/runtime/version.h"
#ifndef NEWOBJ
#define NEWOBJ(x)  \
    newres<x>
#endif
#endif

#if (HHVM_VERSION_MAJOR == 3) && (HHVM_VERSION_MINOR >= 7) 
#include "hphp/runtime/ext/extension-registry.h"
#include "hphp/runtime/base/actrec-args.h"
#endif

namespace HPHP
{

extern Extension* Yaf_Common_GetExtension(const String& ext_name);

extern bool Yaf_Common_InstanceOf(const Object& object, const String& class_name);

}




#endif //_HHVM_COMMON_H
