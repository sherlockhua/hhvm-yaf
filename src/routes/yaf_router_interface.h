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

#define YAF_ROUTE_PROPETY_NAME_MATCH    "_route"
#define YAF_ROUTE_PROPETY_NAME_ROUTE    "_default"
#define YAF_ROUTE_PROPETY_NAME_MAP      "_maps"
#define YAF_ROUTE_PROPETY_NAME_VERIFY   "_verify"

#define YAF_ROUTER_URL_DELIMIETER    "/"                                                                          
#define YAF_ROUTE_REGEX_DILIMITER    '#'
#define YAF_ROUTE_REGEX_DILIMITER_STR    "#"



namespace HPHP
{

extern Variant yaf_route_instance(const Object& object, 
        const Variant& config);

}




#endif //_YAF_ROUTER_INTERFACE_H
