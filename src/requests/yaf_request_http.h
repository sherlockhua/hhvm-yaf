/*===========================================
*   Copyright (C) 2014 All rights reserved.
*   
*   company      : xiaomi
*   author       : root
*   email        : root@xiaomi.com
*   date         ：2014-12-18 07:28:39
*   description  ：
*
=============================================*/
#ifndef _YAF_REQUEST_HTTP_H_
#define _YAF_REQUEST_HTTP_H_

#include "ext_yaf.h"

namespace HPHP {
extern Variant yaf_request_http_instance(Object* object, const char* request_uri, const char* base_uri);

}

#endif //_YAF_REQUEST_HTTP_H
