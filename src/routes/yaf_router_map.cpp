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

void YafExtension::_initYafMapRouterClass()
{
    HHVM_ME(Yaf_Route_Map, __construct);
    HHVM_ME(Yaf_Route_Map, route);
}
 
}
