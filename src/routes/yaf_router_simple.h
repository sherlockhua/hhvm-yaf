/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:05:19
*   description  ：
*
=============================================*/
#ifndef _YAF_ROUTER_SIMPLE_H_
#define _YAF_ROUTER_SIMPLE_H_

#include "ext_yaf.h"

namespace HPHP {

#define YAF_ROUTE_SIMPLE_VAR_NAME_MODULE        "module" 
#define YAF_ROUTE_SIMPLE_VAR_NAME_CONTROLLER    "controller"
#define YAF_ROUTE_SIMPLE_VAR_NAME_ACTION        "action"

extern Variant yaf_route_simple_instance(const Object& object, 
        const String& module, const String& controller, const String& action);

}






#endif //_YAF_ROUTER_SIMPLE_H
