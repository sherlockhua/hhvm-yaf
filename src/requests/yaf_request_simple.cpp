/*===========================================
*   Copyright (C) 2014 All rights reserved.
*   
*   company      : xiaomi
*   author       : root
*   email        : root@xiaomi.com
*   date         ：2014-12-18 07:29:18
*   description  ：
*
=============================================*/
#include "yaf_request_simple.h"
#include "ext_yaf.h"
#include "yaf_request.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/base/php-globals.h"


namespace HPHP{

static Variant HHVM_METHOD(Yaf_Request_Simple, getQuery, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_GET).toArray().exists(name)) {
        return def;
    }

    return php_global(S_GET).toArray()[name];
}

static Variant HHVM_METHOD(Yaf_Request_Simple, getRequest, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_REQUEST).toArray().exists(name)) {
        return def;
    }

    return php_global(S_REQUEST).toArray()[name];
}

static Variant HHVM_METHOD(Yaf_Request_Simple, getPost, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_POST).toArray().exists(name)) {
        return def;
    }

    return php_global(S_POST).toArray()[name];
}
 
static Variant HHVM_METHOD(Yaf_Request_Simple, getCookie, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_COOKIE).toArray().exists(name)) {
        return def;
    }

    return php_global(S_COOKIE).toArray()[name];
}

static Variant HHVM_METHOD(Yaf_Request_Simple, getFiles, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_FILES).toArray().exists(name)) {
        return def;
    }

    return php_global(S_FILES).toArray()[name];
}

static Variant HHVM_METHOD(Yaf_Request_Simple, get, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    auto paramsTmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_PARAMS, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Simple");

    if (paramsTmp->isNull()) {
        return def;
    }

    Array& params = paramsTmp->toArrRef();
    if (params.exists(name)) {
        return params[name];
    }

    if (php_global(S_POST).toArray().exists(name)) {
        return php_global(S_POST).toArray()[name];
    }

    if (php_global(S_GET).toArray().exists(name)) {
        return php_global(S_GET).toArray()[name];
    }

    if (php_global(S_COOKIE).toArray().exists(name)) {
        return php_global(S_COOKIE).toArray()[name];
    }

    if (php_global(S_SERVER).toArray().exists(name)) {
        return php_global(S_SERVER).toArray()[name];
    }

    return def;
}
 
static bool HHVM_METHOD(Yaf_Request_Simple, isXmlHttpRequest)
{
    if (!php_global(S_SERVER).toArray().exists(String("HTTP_X_REQUESTED_WITH"))) {
        return false;
    }

    Variant header = php_global(S_SERVER).toArray()[String("HTTP_X_REQUESTED_WITH")];
    if (!header.isString()) {
        return false;
    }

    std::string str_header = header.toString().toCppString();
    if (strncasecmp(str_header.c_str(), "XMLHttpRequest", str_header.length())) {
        return false;
    } 

    return true;
}

static void HHVM_METHOD(Yaf_Request_Simple, __construct, 
        const Variant& method, const Variant& module,
        const Variant& controller, const Variant& action,
        const Variant& params)
{
    if (!params.isNull() && !params.isArray()) {
        raise_error("expect params is an array");
        return;
    } 

    auto ptr_method = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_METHOD, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Simple");

    auto ptr_module = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Simple");

    auto ptr_controller = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_CONTROLLER, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Simple");

    auto ptr_action = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_ACTION, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Simple");

    auto ptr_routed = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_ROUTED, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Simple");
    //TODO not finished
    if (method.isNull() || !method.isString()) {
        if (php_global(S_SERVER).toArray().exists(String("HTTP_REQUEST_METHOD"))) {
            Variant request_method = php_global(S_SERVER).toArray()[String("HTTP_REQUEST_METHOD")];
            *ptr_method = request_method;
        } else {
            if (strncmp(RuntimeOption::ExecutionMode, "cli", 3) == 0) {
                *ptr_method = String("CLI");
            } else {
                *ptr_method = String("Unknow");
            }
        }
    } else {
        *ptr_method = method;
    }

    if (!module.isNull() || !controller.isNull() || !action.isNull()) {
        if (module.isNull() || !module.isString()) {
            *ptr_module = String(g_yaf_local_data.get()->default_module.c_str()); 
        } else {
            *ptr_module = module;
        }

        if (controller.isNull() || !controller.isString()) {
            *ptr_controller = String(g_yaf_local_data.get()->default_controller.c_str());
        } else {
            *ptr_controller = controller;
        }

        if (action.isNull() || !action.isString()) {
            *ptr_action = String(g_yaf_local_data.get()->default_action.c_str());
        } else {
            *ptr_action = action;
        }
        *ptr_routed = Variant(true);
    } else {
        std::string str_query;
        if (php_global(S_SERVER).toArray().exists(String("argv"))) {
            Variant argv = php_global(S_SERVER).toArray()[String("argv")];
            if (argv.isArray()) {
                const Array argv_array = argv.toCArrRef();
                ArrayIter iter = argv_array.begin();
                while (!iter.end()) {
                    Variant value = iter.second();
                    if (!value.isString()) {
                        iter.next();
                        continue;
                    }

                    std::string str_value = value.toString().toCppString();
                    if (strncasecmp(str_value.c_str(), YAF_REQUEST_SERVER_URI, sizeof(YAF_REQUEST_SERVER_URI) - 1)) {
                        iter.next();
                        continue;
                    }

                    str_query = std::string(str_value, sizeof(YAF_REQUEST_SERVER_URI));
                    break;
                }
            }
        }

        auto ptr_uri = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_URI, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Simple");

        if (!str_query.length()) {
            *ptr_uri = String("");
        } else {
            *ptr_uri = String(str_query.c_str());
        }
    }

    auto ptr_params = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_PARAMS, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Http");

    if (params.isNull() || !params.isArray()) {
        *ptr_params = Array::Create();
    } else {
        *ptr_params = params;
    }
}

static void HHVM_METHOD(Yaf_Request_Simple, __clone)
{
}




void YafExtension::_initYafRequestSimpleClass()
{
    HHVM_ME(Yaf_Request_Simple, getQuery);
    HHVM_ME(Yaf_Request_Simple, getRequest);
    HHVM_ME(Yaf_Request_Simple, getPost);
    HHVM_ME(Yaf_Request_Simple, getCookie);
    HHVM_ME(Yaf_Request_Simple, getFiles);
    HHVM_ME(Yaf_Request_Simple, get);
    HHVM_ME(Yaf_Request_Simple, isXmlHttpRequest);
    HHVM_ME(Yaf_Request_Simple, __construct);
    HHVM_ME(Yaf_Request_Simple, __clone);

}

}
