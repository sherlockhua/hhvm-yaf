/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-22 16:39:33
*   description  ：
*
=============================================*/
#ifndef _YAF_CONFIG_H_
#define _YAF_CONFIG_H_

#include "hphp/runtime/base/base-includes.h"

namespace HPHP {

#define YAF_EXTRACT_FUNC_NAME           "extract"
#define YAF_CONFIG_PROPERT_NAME         "_config" 
#define YAF_CONFIG_PROPERT_NAME_READONLY "_readonly"
#define YAF_CONFIG_PROPERT_NAME_CURSOR "_cursor"

class yaf_config_cursor : public SweepableResourceData {
public:

    DECLARE_RESOURCE_ALLOCATION(yaf_config_cursor)
    CLASSNAME_IS("yaf_config_cursor")
    virtual const String& o_getClassNameHook() const { return classnameof(); }

    explicit yaf_config_cursor(ArrayIter iter) : _iter(iter) {}
    virtual ~yaf_config_cursor() { Close(); }
    void Close() {
    }
  
    ArrayIter& cursor() { return _iter; }

private:
    ArrayIter _iter;
};


}





#endif //_YAF_CONFIG_H
