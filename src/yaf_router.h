/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-30 11:13:14
*   description  ：
*
=============================================*/
#ifndef _YAF_ROUTER_H_
#define _YAF_ROUTER_H_

#include "ext_yaf.h"

#define YAF_ROUTER_DEFAULT_ACTION       "index"
#define YAF_ROUTER_DEFAULT_CONTROLLER   "Index"
#define YAF_ROUTER_DEFAULT_MODULE       "Index"
#define YAF_DEFAULT_EXT                 "php"

#define YAF_ROUTER_PROPERTY_NAME_ROUTERS        "_routes"
#define YAF_ROUTER_PROPERTY_NAME_CURRENT_ROUTE  "_current"


namespace HPHP
{

extern Variant yaf_router_instance(const Object* object);
extern Array yaf_router_parse_parameters(const char* uri);

}


#endif //_YAF_ROUTER_H
