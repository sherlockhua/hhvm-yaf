/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-03-12 17:54:23
*   description  ：
*
=============================================*/
#ifndef _YAF_SESSION_H_
#define _YAF_SESSION_H_

#include "ext_yaf.h"


#define YAF_SESSION_PROPERTY_NAME_STATUS    "_started"
#define YAF_SESSION_PROPERTY_NAME_SESSION   "_session"
#define YAF_SESSION_PROPERTY_NAME_INSTANCE  "_instance" 


namespace HPHP {

const StaticString  
    s_SESSION("_SESSION");

}




#endif //_YAF_SESSION_H
