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

#define YAF_CONFIG_CACHE_CTIME "ctime"
#define YAF_CONFIG_CACHE_DATA  "data"

#ifdef HHVM_VERSION_3_2_NEW
static Variant yaf_config_instance(ObjectData* object, 
#else
extern Variant yaf_config_instance(Object* object, 
#endif
        const Variant& filename, const Variant& section);

#ifdef HHVM_VERSION_3_2_NEW
Variant yaf_config_simple_instance(ObjectData* object, 
#else
Variant yaf_config_simple_instance(const Object* object, 
#endif
        const Variant& config, const Variant& readonly);

#ifdef HHVM_VERSION_3_2_NEW
extern Variant yaf_config_ini_instance(ObjectData* object, 
#else
extern Variant yaf_config_ini_instance(const Object* object, 
#endif
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
