/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:05:45
*   description  ：
*
=============================================*/
#ifndef _YAF_ROUTER_STATIC_H_
#define _YAF_ROUTER_STATIC_H_

#include "ext_yaf.h"

namespace HPHP {

extern bool yaf_route_pathinfo_route(const Object& request, 
        const char* str_req_uri, int str_req_uri_len);
}





#endif //_YAF_ROUTER_STATIC_H
