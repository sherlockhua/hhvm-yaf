/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:02:03
*   description  ：
*
=============================================*/
#include "yaf_router_map.h"

namespace HPHP {

Variant yaf_route_map_instance(const Object* data)
{
    return init_null_variant;
}

static void HHVM_METHOD(Yaf_Route_Map, __construct, const Variant& controller_prefer, const Variant& delim)
{
    (void)yaf_route_map_instance(&this_);
}

static Variant HHVM_METHOD(Yaf_Route_Map, route, const Variant& request)
{
    return init_null_variant;
}



void YafExtension::_initYafMapRouterClass()
{
    HHVM_ME(Yaf_Route_Map, __construct);
    HHVM_ME(Yaf_Route_Map, route);
}
 
}
