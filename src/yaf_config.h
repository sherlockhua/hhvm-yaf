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

#include "yaf_common.h"

namespace HPHP {

#define YAF_EXTRACT_FUNC_NAME           "extract"
#define YAF_CONFIG_PROPERT_NAME         "_config" 
#define YAF_CONFIG_PROPERT_NAME_READONLY "_readonly"
#define YAF_CONFIG_PROPERT_NAME_CURSOR "_cursor"

#define YAF_CONFIG_CACHE_CTIME "ctime"
#define YAF_CONFIG_CACHE_DATA  "data"

extern Variant yaf_config_instance(Object& object, 
        const Variant& filename, const Variant& section);

Variant yaf_config_simple_instance(const Object& object, 
        const Variant& config, const Variant& readonly);

extern Variant yaf_config_ini_instance(const Object& object, 
        const Variant& filename, const Variant& section);




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
