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
#include "yaf_router_rewrite.h"
#include "yaf_router_map.h"
#include "yaf_router_simple.h"
#include "yaf_router_supervar.h"

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

    Object tmp_null = null_object;
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

        return yaf_route_regex_instance(tmp_null, var_match, var_def, 
                var_map, init_null_variant);
    }

    if (strncasecmp(type.c_str(), "rewrite", sizeof("rewrite") - 1) == 0) {
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

        return yaf_route_rewrite_instance(tmp_null, var_match, var_def.toArray(), init_null_variant);
    }

    if (strncasecmp(type.c_str(), "map", sizeof("map") - 1) == 0) {
        Variant prefer(false);
        Variant delim(init_null_variant);
        if (arr.exists(String("controllerPrefer"))) { 
            prefer = arr[String("controllerPrefer")].toBoolean();
        }

        if (arr.exists(String("delimiter"))) {
            delim = arr[String("delimiter")];
        }

        return yaf_route_map_instance(tmp_null, prefer, delim);
    }

    if (strncasecmp(type.c_str(), "simple", sizeof("simple") - 1) == 0) {
        Variant module;
        Variant controller;
        Variant action;

        if (arr.exists(String("module")) == false) { 
            return init_null_variant;
        }

        module = arr[String("module")];
        if (module.isString() == false) {
            return init_null_variant;
        }

        if (arr.exists(String("controller")) == false) { 
            return init_null_variant;
        }

        controller = arr[String("controller")];
        if (controller.isString() == false) {
            return init_null_variant;
        }

        if (arr.exists(String("action")) == false) { 
            return init_null_variant;
        }

        action = arr[String("action")];
        if (action.isString() == false) {
            return init_null_variant;
        }

        return yaf_route_simple_instance(tmp_null, module.toString(), 
                controller.toString(), action.toString());
    }

    if (strncasecmp(type.c_str(), "supervar", sizeof("supervar") - 1) == 0) {

        Variant supervar;
        if (arr.exists(String("varname")) == false) { 
            return init_null_variant;
        }

        supervar = arr[String("varname")];
        return yaf_route_supervar_instance(tmp_null, supervar.toString());
    }

    return  init_null_variant;
}

}
