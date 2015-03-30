/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:02:03
*   description  ：
*
=============================================*/
#include "yaf_router_map.h"
#include "yaf_request.h"
#include "yaf_router.h"
#include "yaf_router_interface.h"

namespace HPHP {

Variant yaf_route_map_instance(const Object* data, 
        const Variant& controller_prefer, const Variant& delim)
{
    Object o;
    if (data == NULL) {
        Array arr = Array::Create();
        arr.append(controller_prefer);
        arr.append(delim);

        o = createObject("Yaf_Route_Map", arr) ;
    } else {
        o = *data;
    }

    if (controller_prefer.isBoolean()) {
        auto ptr_prefer = o->o_realProp(YAF_ROUTE_MAP_VAR_NAME_CTL_PREFER, 
            ObjectData::RealPropUnchecked, "Yaf_Route_Map");
        *ptr_prefer = controller_prefer;
    }

    if (delim.isString()) {
        auto ptr_delim = o->o_realProp(YAF_ROUTE_MAP_VAR_NAME_DELIMETER, 
            ObjectData::RealPropUnchecked, "Yaf_Route_Map");
        *ptr_delim = delim;
    }

    return o;
}

static void HHVM_METHOD(Yaf_Route_Map, __construct, 
        const Variant& controller_prefer, const Variant& delim)
{
    (void)yaf_route_map_instance(&this_, controller_prefer, delim);
}


static bool yaf_route_map_route(const Object& o, const Object& request)
{
    auto ptr_uri = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_URI, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto ptr_base_uri = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_BASE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto ptr_ctl_prefer = o->o_realProp(YAF_ROUTE_MAP_VAR_NAME_CTL_PREFER, 
             ObjectData::RealPropUnchecked, "Yaf_Route_Map");

    auto ptr_delim = o->o_realProp(YAF_ROUTE_MAP_VAR_NAME_DELIMETER, 
             ObjectData::RealPropUnchecked, "Yaf_Route_Map");

    if (ptr_uri == NULL) {
        raise_warning("invalid uri:%p", ptr_uri);
        return false;
    }

    std::string req_uri;
    if (ptr_uri && ptr_uri->isString() && 
        ptr_base_uri && 
        ptr_base_uri->isString()&&
        strncasecmp(ptr_uri->toString().c_str(), 
            ptr_base_uri->toString().c_str(), ptr_uri->toString().length()) == 0) {
        req_uri = std::string(ptr_uri->toString().c_str() + ptr_base_uri->toString().length()); 
    } else {
        req_uri = std::string(ptr_uri->toString().c_str());
    }

    if (req_uri.length() == 0) {
        return false;
    }

    std::string str_query_str;
    if (ptr_delim->isString() && ptr_delim->toString().length()) {
        const char* str_delim = ptr_delim->toString().c_str();
        char* tmp_req_uri = strdup(req_uri.c_str());
        char* query_str = strstr(tmp_req_uri, str_delim);
        if (query_str && *(query_str - 1) == '/') {
            char* rest = query_str + strlen(str_delim);
            if (*rest == '\0') {
                req_uri = std::string(req_uri, query_str - tmp_req_uri);
                query_str = NULL;
            } else if(*rest == '/') {
                req_uri = std::string(req_uri, query_str - tmp_req_uri);
                str_query_str  = std::string(rest);
            } else {
                query_str = NULL;
            }
        }
        
        free(tmp_req_uri);
    }

    std::string route_result;
    char* save_ptr = NULL;
    char* tmp = strdup(req_uri.c_str());
    char* seg = strtok_r(tmp, YAF_ROUTER_URL_DELIMIETER, &save_ptr);
    while (seg) {
        int seg_len = strlen(seg);
        if (seg_len) {
            route_result += seg;
        }

        route_result += "_";
        seg = strtok_r(NULL, YAF_ROUTER_URL_DELIMIETER, &save_ptr);
    }

    free(tmp);
    if (route_result.length()) {
        if (ptr_ctl_prefer->toBoolean()) {
            auto ptr_controller = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_CONTROLLER, 
                        ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
            *ptr_controller = String(route_result);
        } else {
            auto ptr_action = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_ACTION, 
                        ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
            *ptr_action = String(route_result);
        }
    }

    if (str_query_str.length()) {
        Array arr = yaf_router_parse_parameters(str_query_str.c_str()); 
        yaf_request_set_params_multi(&request, arr);
    }

    return true;
}

static Variant HHVM_METHOD(Yaf_Route_Map, route, const Variant& request)
{
    if (!request.isObject()) {
        return false;
    }

    if (!request.toObject()->o_instanceof("Yaf_Request_Abstract")) {
        return false;
    }

    return yaf_route_map_route(this_, request.toObject());
}



void YafExtension::_initYafMapRouterClass()
{
    HHVM_ME(Yaf_Route_Map, __construct);
    HHVM_ME(Yaf_Route_Map, route);
}
 
}
