/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:05:54
*   description  ：
*
=============================================*/
#include "yaf_router_static.h"
#include "yaf_application.h"
#include "yaf_request.h"
#include "yaf_router.h"
#include "ext_yaf.h"


namespace HPHP {

static bool yaf_route_pathinfo_route(const Object& request, 
        const char* str_req_uri, int str_req_uri_len)
{
    Array params;
    char* req_uri;
    int req_uri_len = 0;

    //char *module = NULL, *controller = NULL, *action = NULL, *rest = NULL;
    std::string module;
    std::string controller;
    std::string action;
    std::string rest;

    std::string str_tmp(str_req_uri, str_req_uri_len);

    req_uri = (char*)str_tmp.c_str();
    req_uri_len = str_tmp.length();

    do {
#define strip_slashs(p) while (*p == ' ' || *p == '/') { ++p; }
        char *s, *p;
        char *uri;

        if (req_uri_len == 0
                || (req_uri_len == 1 && *req_uri == '/')) {
            break;
        }

        uri = req_uri;
        s = p = uri;

        if (req_uri_len) {
            char *q = req_uri + req_uri_len - 1;
            while (q > req_uri && (*q == ' ' || *q == '/')) {
                *q-- = '\0';
            }
        }

        strip_slashs(p);

        if ((s = strstr(p, "/")) != NULL) {
            String tmp_module(std::string(p, s - p)) ;
            if (yaf_application_is_module_name(tmp_module)) {
                module = std::string(tmp_module.c_str(), tmp_module.length());
                p  = s + 1;
                strip_slashs(p);
                if ((s = strstr(p, "/")) != NULL) {
                    controller = std::string(p, s - p);
                    p  = s + 1;
                }
            } else {
                controller = std::string(p, s - p);
                p  = s + 1;
            }
        }

        strip_slashs(p);
        if ((s = strstr(p, "/")) != NULL) {
            action = std::string(p, s - p);
            p  = s + 1;
        }

        strip_slashs(p);
        if (*p != '\0') {
            do {
                if (!module.length() && !controller.length() && !action.length()) {
                    String tmp_module(std::string(p, strlen(p))) ;
                    if (yaf_application_is_module_name(tmp_module)) {
                        module = std::string(p);
                        break;
                    }
                }

                if (!controller.length()) {
                    controller = std::string(p);
                    break;
                }

                if (!action.length()) {
                    action = std::string(p);
                    break;
                }

                rest = std::string(p);
            } while (0);
        }

        if (module.length() && controller.length() == 0) {
            controller = module;
            module.clear();
        } else if (module.length() && action.length() == 0) {
            action = controller;
            controller = module;
            module.clear() ;
        } else if (controller.length() && action.length() == 0 ) {
            /* /controller */
            //if (YAF_G(action_prefer)) {
            if (g_yaf_local_data.get()->action_prefer) {
                action = controller;
                controller.clear(); 
            }
        }
    } while (0);

    if (module.length()) {
        auto ptr_module = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

        *ptr_module = String(module);
    }
    if (controller.length()) {
        auto ptr_controller = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_CONTROLLER, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

        *ptr_controller = String(controller);
    }

    if (action.length()) {
        auto ptr_action = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_ACTION, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

        *ptr_action = String(action);
    }

    if (rest.length()) {
        params = yaf_router_parse_parameters(rest.c_str());
        (void)yaf_request_set_params_multi(&request, params);
    }

    return true;
}

static bool yaf_route_static_route(const Object& route, const Object& request)
{
    //zval *zuri, *base_uri;
    const char *req_uri;
    int  req_uri_len;

    auto ptr_uri = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_URI, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    auto ptr_base_uri = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_BASE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (ptr_base_uri->isString() 
            && !strncasecmp(ptr_uri->toString().c_str(), 
                ptr_base_uri->toString().c_str(), ptr_base_uri->toString().length())) {
        req_uri = ptr_uri->toString().c_str() + ptr_base_uri->toString().length();
        req_uri_len = ptr_uri->toString().length();
    } else {
        req_uri = ptr_uri->toString().c_str();
        req_uri_len = ptr_uri->toString().length();
    }

    yaf_route_pathinfo_route(request, req_uri, req_uri_len);

    return true;
}

static Variant HHVM_METHOD(Yaf_Route_Static, route, const Variant& request)
{
    if (!request.isObject()) {
        yaf_trigger_error(YAF_ERR_ROUTE_FAILED, 
                "trigger error failed, request is not object");
        return false;
    }

    return yaf_route_static_route(this_, request.toObject());
}

static Variant HHVM_METHOD(Yaf_Route_Static, match)
{
    return true;
}

void YafExtension::_initYafStaticRouterClass()
{
    //HHVM_ME(Yaf_Route_Static, __construct);
    HHVM_ME(Yaf_Route_Static, route);
    HHVM_ME(Yaf_Route_Static, match);
}
 

}
