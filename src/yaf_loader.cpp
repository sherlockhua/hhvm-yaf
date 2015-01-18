/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-14 20:42:47
*   description  ：
*
=============================================*/
#include "yaf_loader.h"
#include "ext_yaf.h"

namespace HPHP {

Variant yaf_loader_import(const char *path, int len, int use_path)
{ 
    //return require(String(path), true, g_context->getCwd().data(), true);
    
    String func("yaf_loader_import");

    Array params = Array::Create();
    params.append(String(path));
    return vm_call_user_func(func, params);
}

}
