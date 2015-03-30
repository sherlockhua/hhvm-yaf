/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:05:03
*   description  ：
*
=============================================*/
#include "hphp/runtime/base/preg.h"
#include "yaf_router_rewrite.h"
#include "hphp/runtime/base/php-globals.h"
#include "yaf_application.h"
#include "ext_yaf.h"
#include "yaf_request.h"
#include "yaf_router.h"
#include "yaf_router_interface.h"


namespace HPHP {


static Variant yaf_route_rewrite_match(const Object& o, 
        const char* req_uri, int req_uri_len)
{
    char match[8192];
    auto ptr_match = o->o_realProp(YAF_ROUTE_PROPETY_NAME_MATCH,
                    ObjectData::RealPropUnchecked, "Yaf_Route_Rewrite");
    snprintf(match, sizeof(match), "%s", ptr_match->toString().c_str());

    std::string pattern;
    pattern.append(YAF_ROUTE_REGEX_DILIMITER_STR);
    pattern.append("^");

    char* save_ptr;
    char* seg = strtok_r(match, YAF_ROUTER_URL_DELIMIETER, &save_ptr);
    while (seg != NULL) {
        int len = strlen(seg);
        if (len) {
            pattern.append(YAF_ROUTER_URL_DELIMIETER);
            if (*seg == '*') {
                pattern.append("(?P<__yaf_route_rest>.*)");
                break;
            }

            if (*seg == ':') {
                pattern.append("(?P<");
                pattern.append(std::string(seg+1));
                pattern.append(std::string(">[^") + YAF_ROUTER_URL_DELIMIETER + "]+)");
            } else {
                pattern.append(seg);
            }
        }

        seg = strtok_r(NULL, YAF_ROUTER_URL_DELIMIETER, &save_ptr);
    }

    pattern.append(YAF_ROUTE_REGEX_DILIMITER_STR);
    pattern.append("i");

    Variant matches;
    Variant ret = preg_match_all(String(pattern), 
                   String(std::string(req_uri, req_uri_len)), matches);
    int match_count = 0;
    if (ret.isIntVal() ) {
        match_count = ret.toInt32();
    }

    if (match_count <= 0) {
        raise_warning("match failed");
        return init_null_variant;
    }

    if (!matches.isArray()) {
        return init_null_variant;
    }

    Array args = Array::Create();
    Array& arr_matches = matches.toArrRef();
    ArrayIter iter = arr_matches.begin();
    while (!iter.end()) {
        Variant key = iter.first();
        Variant value = iter.second();
        
        if (!key.isString()) {
            iter.next();
            continue;
        }

        if (!strncmp(key.toString().c_str(), "__yaf_route_rest", key.toString().length())) {
            Variant retval = yaf_router_parse_parameters(value.toArrRef()[0].toString().c_str());
            if (retval.isArray()) {
                args.merge(retval.toArray());
            }
        } else {
            args.set(key, value.toArrRef()[0]);
        }

        iter.next();
    }

    return args;
}

static bool yaf_route_rewrite_route(const Object& o, const Object& request)
{
    char req_uri[8192];

    auto ptr_req_uri = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_URI,
                    ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto ptr_req_baseuri = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_BASE,
                    ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (ptr_req_uri == NULL) {
        raise_warning("invalid ptr_uri:%p", ptr_req_uri);
        return false;
    }

    if (ptr_req_baseuri->isString() 
            && !strncasecmp(ptr_req_uri->toString().c_str(), 
                ptr_req_baseuri->toString().c_str(), ptr_req_baseuri->toString().length())) {

        const char* tmp_req_uri = ptr_req_uri->toString().c_str() + ptr_req_baseuri->toString().length();
        snprintf(req_uri, sizeof(req_uri), "%s", tmp_req_uri);
    } else {
        const char* tmp_req_uri = ptr_req_uri->toString().c_str();
        snprintf(req_uri, sizeof(req_uri), "%s", tmp_req_uri);
    }

    Variant ret = yaf_route_rewrite_match(o, req_uri, strlen(req_uri));
    if (ret.isNull()) {
        return false;
    }

    auto ptr_route = o->o_realProp(YAF_ROUTE_PROPETY_NAME_ROUTE,
                    ObjectData::RealPropUnchecked, "Yaf_Route_Rewrite");
    Array& route = ptr_route->toArrRef();
    if (route.exists(String("module"))) {
        auto ptr_module = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE,
                    ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *ptr_module = route[String("module")];
    }

    if (route.exists(String("controller"))) {
        auto ptr_controller = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_CONTROLLER,
                    ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *ptr_controller = route[String("controller")];
    }

    if (route.exists(String("action"))) {
        auto ptr_action = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_ACTION,
                    ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
        *ptr_action = route[String("action")];
    }

    (void)yaf_request_set_params_multi(&request, ret.toArray());
    return true;
}

Variant yaf_route_rewrite_instance( const Object* object,
        const Variant& match, const Array& route, const Variant& verify)
{
    Object o;
    if (object == NULL) {
        Array params = Array::Create();
        params.append(match);
        params.append(route);
        params.append(verify);

        o = createObject("Yaf_Route_Rewrite", params);
    }  else {
        o = *object;
    }

    auto ptr_match = o->o_realProp(YAF_ROUTE_PROPETY_NAME_MATCH,
                    ObjectData::RealPropUnchecked, "Yaf_Route_Rewrite");

    auto ptr_route = o->o_realProp(YAF_ROUTE_PROPETY_NAME_ROUTE,
                    ObjectData::RealPropUnchecked, "Yaf_Route_Rewrite");

    auto ptr_verify = o->o_realProp(YAF_ROUTE_PROPETY_NAME_VERIFY,
                    ObjectData::RealPropUnchecked, "Yaf_Route_Rewrite");

    *ptr_match = match;
    *ptr_route = route;
    *ptr_verify = verify;

    return o;
}

static void HHVM_METHOD(Yaf_Route_Rewrite, __construct, 
        const Variant& match, const Array& route, 
        const Variant& verify)
{
    if (!match.isString() || 
            (match.isString() && match.toString().length() == 0)) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, "Yaf_Route_Rewrite, "\
                "Expects a valid string as the first parameter");
        return;
    }

    if (!verify.isNull() && !verify.isArray()) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR,
                "Yaf_Route_Rewrite, Expects an array as third parameter");
        return;
    }

    (void)yaf_route_rewrite_instance(&this_, match, route, verify);
}

static Variant HHVM_METHOD(Yaf_Route_Rewrite, route, const Variant& request)
{
    if (!request.isObject()) {
        return false;
    }

    if (!request.toObject()->o_instanceof("Yaf_Request_Abstract")) {
        return false;
    }

    return yaf_route_rewrite_route(this_, request.toObject());
}

void YafExtension::_initYafRewriteRouterClass()
{
    HHVM_ME(Yaf_Route_Rewrite, __construct);
    HHVM_ME(Yaf_Route_Rewrite, route);
}

}
