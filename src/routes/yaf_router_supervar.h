/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:06:10
*   description  ：
*
=============================================*/
#ifndef _YAF_ROUTER_SUPERVAR_H_
#define _YAF_ROUTER_SUPERVAR_H_

#include "ext_yaf.h"

namespace HPHP {

#define YAF_ROUTE_SUPERVAR_PROPETY_NAME_VAR "_var_name"

extern Variant yaf_route_supervar_instance(const Object* o, const Variant& name);
}






#endif //_YAF_ROUTER_SUPERVAR_H
