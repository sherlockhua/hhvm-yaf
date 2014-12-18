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
#ifndef _YAF_REQUEST_H_
#define _YAF_REQUEST_H_

#include "ext_yaf.h"

namespace HPHP {

#define YAF_REQUEST_PROPERTY_NAME_MODULE        "module"
#define YAF_REQUEST_PROPERTY_NAME_CONTROLLER    "controller"
#define YAF_REQUEST_PROPERTY_NAME_ACTION        "action"
#define YAF_REQUEST_PROPERTY_NAME_METHOD        "method"
#define YAF_REQUEST_PROPERTY_NAME_PARAMS        "params"
#define YAF_REQUEST_PROPERTY_NAME_URI       "uri"
#define YAF_REQUEST_PROPERTY_NAME_STATE     "dispatched"
#define YAF_REQUEST_PROPERTY_NAME_LANG      "language"
#define YAF_REQUEST_PROPERTY_NAME_ROUTED        "routed"
#define YAF_REQUEST_PROPERTY_NAME_BASE      "_base_uri"
#define YAF_REQUEST_PROPERTY_NAME_EXCEPTION  "_exception"

extern int yaf_request_set_dispatched(Object request, int flag);



}

#endif //_YAF_REQUEST_H
