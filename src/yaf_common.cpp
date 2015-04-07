/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-04-07 14:14:53
*   description  ：
*
=============================================*/
#include "yaf_common.h"

namespace HPHP {

Extension* Yaf_Common_GetExtension(const String& name)
{
#if (HHVM_VERSION_MAJOR == 3) && (HHVM_VERSION_MINOR >= 7) 
    return ExtensionRegistry::get(name);
#else
    return Extension::GetExtension(name);
#endif
}


bool Yaf_Common_InstanceOf(const Object& object, const String& class_name)
{
#if (HHVM_VERSION_MAJOR == 3) && (HHVM_VERSION_MINOR >= 5) 
    return object->instanceof(class_name);
#else
    return object->o_instanceof(class_name);
#endif

}

}
