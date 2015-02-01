/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-01 20:37:59
*   description  ：
*
=============================================*/
#ifndef _YAF_ROUTER_INTERFACE_H_
#define _YAF_ROUTER_INTERFACE_H_

#include "ext_yaf.h"


namespace HPHP
{

extern Variant yaf_route_instance(const Object* object, 
        const Variant& config);

}




#endif //_YAF_ROUTER_INTERFACE_H
