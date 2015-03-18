/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:04:25
*   description  ：
*
=============================================*/
#include "hphp/runtime/base/preg.h"
#include "yaf_router_interface.h"
#include "yaf_router_regex.h"
#include "yaf_application.h"
#include "yaf_request.h"
#include "yaf_router.h"

namespace HPHP {


static Variant yaf_route_regex_match(const Object& object, char* uri, int uri_length)
{
    if (uri_length == 0) {
        return init_null_variant;
    } 

    auto ptr_match = object->o_realProp(YAF_ROUTE_PROPETY_NAME_MATCH, 
                        ObjectData::RealPropUnchecked, "Yaf_Route_Regex");

    auto ptr_map = object->o_realProp(YAF_ROUTE_PROPETY_NAME_MAP, 
                        ObjectData::RealPropUnchecked, "Yaf_Route_Regex");


    Variant matches;
    String str_search(std::string(uri, uri_length));
    Variant ret = preg_match_all(ptr_match->toString(), str_search, matches);

    int match = 0;
    if (ret.isIntVal() ) {
        match = ret.toInt32();
    }

    if (match <= 0) {
        raise_warning("match failed");
        return init_null_variant;
    }

    if (!matches.isArray() || !ptr_map->isArray()) {
        return init_null_variant;
    }

    Array& arr_map = ptr_map->toArrRef();
    Array& arr_matches = matches.toArrRef();
    Array result = Array::Create();

    ArrayIter iter = arr_matches.begin();
    while (!iter.end()) {
        Variant key = iter.first();
        Variant value = iter.second();

        if (arr_map.exists(key)) {
            Variant name = arr_map[key];
            result.set(name, value.toArrRef()[0]);
        }
        iter.next();
    }

    return result;
}
    
static int yaf_route_regex_route(const Object& object, const Object& request)
{
    char* request_uri = NULL;
    auto ptr_uri = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_URI, 
                        ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto ptr_base_uri = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_BASE, 
                        ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    

    if (ptr_base_uri->isString() && 
            strncasecmp(ptr_uri->toString().c_str(), 
                ptr_base_uri->toString().c_str(),
                ptr_base_uri->toString().length()) == 0) {
        request_uri = strdup(ptr_uri->toString().c_str() + ptr_base_uri->toString().length());
    } else {
        request_uri = strdup(ptr_uri->toString().c_str());
    }

    Variant ret = yaf_route_regex_match(object, request_uri, strlen(request_uri));
    if (ret.isNull()) {
        free(request_uri);
        raise_warning("match failed");
        return HHVM_YAF_FAILED;
    }

    auto ptr_routes = object->o_realProp(YAF_ROUTE_PROPETY_NAME_ROUTE, 
                        ObjectData::RealPropUnchecked, "Yaf_Route_Regex");
    if (!ptr_routes->isArray()) {
        free(request_uri);
        raise_warning("regex route be array");
        return HHVM_YAF_FAILED;
    }
    
    Array& arr_routes = ptr_routes->toArrRef();
    if (arr_routes.exists(String("module"))) {
        Variant var_module = arr_routes[String("module")];
        auto ptr_module = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE, 
                        ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *ptr_module = var_module;
    }

    if (arr_routes.exists(String("controller"))) {
        Variant var_ctrl = arr_routes[String("controller")];
        auto ptr_ctrl = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_CONTROLLER, 
                        ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *ptr_ctrl = var_ctrl;
    }

    if (arr_routes.exists(String("action"))) {
        Variant var_action = arr_routes[String("action")];
        auto ptr_action= request->o_realProp(YAF_REQUEST_PROPERTY_NAME_ACTION, 
                        ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *ptr_action = var_action;
    }

    (void)yaf_request_set_params_multi(&request, ret.toArrRef());
    free(request_uri);

    return HHVM_YAF_SUCCESS;
}

Variant yaf_route_regex_instance(
        const Object* object, const Variant& route, const Variant& def,
        const Variant& map, const Variant& verify
        )
{
    Object o;
    if (object != NULL) {
        o = *object;
    } else {
        Array params = Array::Create();
        params.append(route);
        params.append(def);
        params.append(map);
        params.append(verify);

        o = createObject(String("Yaf_Route_Regex"), params);
    } 

    auto ptr_match = o->o_realProp(YAF_ROUTE_PROPETY_NAME_MATCH, 
                        ObjectData::RealPropUnchecked, "Yaf_Route_Regex");
    *ptr_match = route;

    auto ptr_def = o->o_realProp(YAF_ROUTE_PROPETY_NAME_ROUTE, 
                        ObjectData::RealPropUnchecked, "Yaf_Route_Regex");
    *ptr_def = def;

    auto ptr_map = o->o_realProp(YAF_ROUTE_PROPETY_NAME_MAP, 
                        ObjectData::RealPropUnchecked, "Yaf_Route_Regex");
    *ptr_map = map;

    auto ptr_verify = o->o_realProp(YAF_ROUTE_PROPETY_NAME_VERIFY, 
                        ObjectData::RealPropUnchecked, "Yaf_Route_Regex");

    *ptr_verify = verify;
    return o;
}


static Variant HHVM_METHOD(Yaf_Route_Regex, route, const Variant& request)
{
    if (!request.isObject()) {
        yaf_trigger_error(YAF_ERR_ROUTE_FAILED, 
                "trigger error failed, request is not object");
        return false;
    }

    if (!request.toObject()->o_instanceof("Yaf_Request_Abstract")) {
        yaf_trigger_error(YAF_ERR_ROUTE_FAILED, 
                "request is not instance of Yaf_Request_Abstract");
        return false;
    }

    int ret = yaf_route_regex_route(this_, request.toObject());
    if (ret != HHVM_YAF_SUCCESS) {
        return false;
    }

    return true;
}

static void HHVM_METHOD(Yaf_Route_Regex, __construct, 
        const Variant& match, const Array& route, 
        const Array& map, const Array& verify)
{
    if (!match.isString() || match.toString().length() == 0) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, "Expects a valid string"\
                " as the first parameter, in Yaf_Route_Regex");
        return;
    }

    (void)yaf_route_regex_instance(&this_, match, route, map, verify);
}

void YafExtension::_initYafRegexRouterClass()
{
    HHVM_ME(Yaf_Route_Regex, __construct);
    HHVM_ME(Yaf_Route_Regex, route);
}
 
}
