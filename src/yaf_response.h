/*===========================================
*   Copyright (C) 2014 All rights reserved.
*   
*   company      : xiaomi
*   author       : root
*   email        : root@xiaomi.com
*   date         ：2014-12-18 07:48:51
*   description  ：
*
=============================================*/
#ifndef _YAF_RESPONSE_H_
#define _YAF_RESPONSE_H_

#include "ext_yaf.h"
namespace HPHP {

#define YAF_RESPONSE_PROPERTY_NAME_HEADER           "_header"
#define YAF_RESPONSE_PROPERTY_NAME_BODY             "_body"                                                       
#define YAF_RESPONSE_PROPERTY_NAME_HEADEREXCEPTION  "_sendheader"
#define YAF_RESPONSE_PROPERTY_NAME_RESPONSECODE     "_response_code"
#define YAF_RESPONSE_PROPERTY_NAME_DEFAULTBODY      "content"
#define YAF_RESPONSE_PROPERTY_NAME_DEFAULTBODYNAME  "DEFAULT_BODY"
    
#define YAF_RESPONSE_REPLACE 0
#define YAF_RESPONSE_PREPEND 1
#define YAF_RESPONSE_APPEND  2

extern int yaf_response_set_redirect(Variant response, const String& location);
extern Variant yaf_response_instance(const Object* object, const char* sapi_name);
extern int yaf_response_send(const Object& object);
extern int yaf_response_clear_body(const Object* object, const Variant& name);
}





#endif //_YAF_RESPONSE_H
