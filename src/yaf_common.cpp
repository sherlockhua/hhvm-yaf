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


StrNR Yaf_Common_GetClassName(const Object& object)
{
#if (HHVM_VERSION_MAJOR == 3) && (HHVM_VERSION_MINOR >= 5) 
    return object->getClassName();
#else
    return object->o_getClassName();
#endif
}


bool yaf_ob_start()
{
    g_context->obStart(); 
    return true;
}

bool yaf_ob_end_clean()
{
#if (HHVM_API_VERSION > 20131007L) 
    g_context->obClean(k_PHP_OUTPUT_HANDLER_START |
                       k_PHP_OUTPUT_HANDLER_CLEAN |
                       k_PHP_OUTPUT_HANDLER_END);
#else
        g_context->obClean();
#endif

    return g_context->obEnd(); 
}

Variant yaf_ob_get_content()
{
    if (g_context->obGetLevel() == false) {
        raise_warning("get level failed");
        return false;
    }

    return g_context->obCopyContents();
}

Variant yaf_ob_get_clean()
{
  String output = yaf_ob_get_content();
  if (!yaf_ob_end_clean()) {
    return false;
  }

  return output;
}

ActRec* Yaf_Common_GetTP()
{
#if (HHVM_API_VERSION > 20131007L) 
    return vmfp();
#else
    return g_context->getFP();
#endif

}


}
