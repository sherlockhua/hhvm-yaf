/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-01-12 11:52:22
*   description  ：
*
=============================================*/
#ifndef _YAF_VIEW_H_
#define _YAF_VIEW_H_

#include "ext_yaf.h"

#define yaf_view_instance yaf_view_simple_instance
#define yaf_view_ce      yaf_view_simple_ce


#define YAF_VIEW_PROPERTY_NAME_TPLVARS  "_tpl_vars"
#define YAF_VIEW_PROPERTY_NAME_TPLDIR   "_tpl_dir"
#define YAF_VIEW_PROPERTY_NAME_OPTS     "_options"

namespace HPHP {

extern Variant yaf_view_simple_instance(const Object& object, const Variant& tpl_dir,
        const Variant& options);
}



#endif //_YAF_VIEW_H
