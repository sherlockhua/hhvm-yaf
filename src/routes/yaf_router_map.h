/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:01:35
*   description  ：
*
=============================================*/
#ifndef _YAF_ROUTER_MAP_H_
#define _YAF_ROUTER_MAP_H_

#include "ext_yaf.h"

#define YAF_ROUTE_MAP_VAR_NAME_DELIMETER    "_delimeter"
#define YAF_ROUTE_MAP_VAR_NAME_CTL_PREFER   "_ctl_router"

namespace HPHP {

extern Variant yaf_route_map_instance(const Object& data, 
        const Variant& controller_prefer, const Variant& delim);

}



#endif //_YAF_ROUTER_MAP_H
