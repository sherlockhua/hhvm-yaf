/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:04:06
*   description  ：
*
=============================================*/
#ifndef _YAF_ROUTER_REGEX_H_
#define _YAF_ROUTER_REGEX_H_

#include "ext_yaf.h"

namespace HPHP {

extern Variant yaf_route_regex_instance(
        const Object* object, const Variant& route, const Variant& def,
        const Variant& map, const Variant& verify
        );

}





#endif //_YAF_ROUTER_REGEX_H
