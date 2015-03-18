/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-01 20:36:35
*   description  ：
*
=============================================*/
#include "yaf_router_interface.h"
#include "yaf_router_regex.h"

namespace HPHP
{

Variant yaf_route_instance(const Object* object, const Variant& config)
{
    if (!config.isArray()) {
        return init_null_variant;
    }

    Array arr = config.toArray();
    if (!arr.exists(String("type"))) {
        return init_null_variant;
    }

    Variant var_type = arr[String("type")];
    if (var_type.isString() == false) {
        return init_null_variant;
    }

    String type = var_type.toString();
    if (strncasecmp(type.c_str(), "regex", sizeof("regex") - 1) == 0) {
        if (arr.exists(String("match")) == false) { 
            raise_warning("not have match");
            return init_null_variant;
        }

        Variant var_match = arr[String("match")];
        if (var_match.isString() == false) {
            return init_null_variant;
        }

        if (arr.exists(String("route")) == false) {
            raise_warning("not have route");
            return init_null_variant;
        }

        Variant var_def = arr[String("route")];
        if (var_def.isArray() == false) {
            return init_null_variant;
        }

        if (arr.exists(String("map")) == false) {
            raise_warning("not have map");
            return init_null_variant;
        }

        Variant var_map = arr[String("map")];
        if (var_map.isArray() == false) {
            return init_null_variant;
        }

        return yaf_route_regex_instance(NULL, var_match, var_def, var_map, init_null_variant);
    }

    return  init_null_variant;
}

}
