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
#ifndef _YAF_SESSION_EX_H_
#define _YAF_SESSION_EX_H_

#include "ext_yaf.h"


#define YAF_SESSION_PROPERTY_NAME_STATUS    "_started"
#define YAF_SESSION_PROPERTY_NAME_SESSION   "_session"
#define YAF_SESSION_PROPERTY_NAME_INSTANCE  "_instance" 

#define YAF_SESSION_PROPERT_NAME_CURSOR "_cursor"

namespace HPHP {

const StaticString  
    s_SESSION("_SESSION");

class yaf_session_cursor : public SweepableResourceData {
public:

    DECLARE_RESOURCE_ALLOCATION(yaf_session_cursor)
    CLASSNAME_IS("yaf_session_cursor")
    virtual const String& o_getClassNameHook() const { return classnameof(); }

    explicit yaf_session_cursor(ArrayIter iter) : _iter(iter) {}
    virtual ~yaf_session_cursor() { Close(); }
    void Close() {
    }
  
    ArrayIter& cursor() { return _iter; }

private:
    ArrayIter _iter;
};


}


#endif //_YAF_SESSION_H
