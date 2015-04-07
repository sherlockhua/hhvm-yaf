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

}
