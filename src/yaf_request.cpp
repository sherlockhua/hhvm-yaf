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
#include "yaf_request.h"
#include "ext_yaf.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/base/php-globals.h"
#include "requests/yaf_request_http.h"

namespace HPHP{

bool yaf_request_is_dispatched(const Object* request)
{
    auto ptr_dispatched = (*request)->o_realProp(YAF_REQUEST_PROPERTY_NAME_STATE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    return ptr_dispatched->toBoolean();
}

bool yaf_request_is_routed(const Object* request)
{
    auto ptr_route = (*request)->o_realProp(YAF_REQUEST_PROPERTY_NAME_ROUTED, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    return ptr_route->toBoolean();
}

int yaf_request_set_params_multi(const Object* request, const Array& params)
{
    auto ptr_params = (*request)->o_realProp(YAF_REQUEST_PROPERTY_NAME_PARAMS, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    *ptr_params = params;

    return HHVM_YAF_SUCCESS;
}

Variant yaf_request_instance(const Object* object, const char* base_uri)
{
    Variant str_base_uri = String(base_uri);
    Variant tmp = init_null_variant;
    return yaf_request_http_instance(object, tmp, str_base_uri);
}

int yaf_request_set_routed(Object* object, int flag)
{
    if (object == NULL) {
        return HHVM_YAF_FAILED;
    }

    auto var_flag = (*object)->o_realProp(YAF_REQUEST_PROPERTY_NAME_ROUTED, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    *var_flag = flag;

    return HHVM_YAF_SUCCESS;
}

int yaf_request_set_dispatched(Object request, int flag)
{
    auto var_flag = request->o_realProp(YAF_REQUEST_PROPERTY_NAME_STATE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    *var_flag = Variant(flag);

    return 0;
}

static bool HHVM_METHOD(Yaf_Request_Abstract, isGet) 
{
    String method = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_METHOD, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract")->toString();

    if (strncasecmp(method.toCppString().c_str(), "GET", method.length())) {
        return true;
    }

    return false;
}

static bool HHVM_METHOD(Yaf_Request_Abstract, isPost) 
{
    String method = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_METHOD, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract")->toString();

    if (strncasecmp(method.toCppString().c_str(), "POST", method.length())) {
        return true;
    }

    return false;
}

static bool HHVM_METHOD(Yaf_Request_Abstract, isPut) 
{
    String method = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_METHOD, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract")->toString();

    if (strncasecmp(method.toCppString().c_str(), "PUT", method.length())) {
        return true;
    }

    return false;
}
 
static bool HHVM_METHOD(Yaf_Request_Abstract, isHead) 
{
    String method = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_METHOD, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract")->toString();

    if (strncasecmp(method.toCppString().c_str(), "HEAD", method.length())) {
        return true;
    }

    return false;
}

static bool HHVM_METHOD(Yaf_Request_Abstract, isOptions) 
{
    String method = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_METHOD, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract")->toString();

    if (strncasecmp(method.toCppString().c_str(), "Options", method.length())) {
        return true;
    }

    return false;
}
 
static bool HHVM_METHOD(Yaf_Request_Abstract, isCli) 
{
    String method = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_METHOD, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract")->toString();

    if (strncasecmp(method.toCppString().c_str(), "Cli", method.length())) {
        return true;
    }

    return false;
}

static bool HHVM_METHOD(Yaf_Request_Abstract, isXmlHttpRequest) 
{
    return false;
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, getServer, const Variant& name,
        const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_SERVER).toArray().exists(name)) {
        return def;
    }

    return php_global(S_SERVER).toArray()[name];
}
 
static Variant HHVM_METHOD(Yaf_Request_Abstract, getEnv, const Variant& name,
    const Variant& def) 
{
    if (name.isNull()) {
        return def;
    }

    if (!php_global(S_ENV).toArray().exists(name)) {
        return def;
    }

    return php_global(S_ENV).toArray()[name];
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, setParam, const Variant& name,
        const Variant& value)
{
    
    auto paramsTmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_PARAMS, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    if (paramsTmp->isNull()) {
        *paramsTmp = Array::Create();
    }

    Array& params = paramsTmp->toArrRef();
    if (name.isArray()) {
        params.merge(name.toCArrRef());
    } else {
        params.set(name.toString(), value);
    }

    return this_;
}
 
static Variant HHVM_METHOD(Yaf_Request_Abstract, getParam, const String& name,
        const Variant& def)
{
    auto paramsTmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_PARAMS, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (paramsTmp->isNull()) {
        return def;
    }

    Array& params = paramsTmp->toArrRef();
    if (params.exists(name)) {
        return params[name];
    }

    return def;
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, getParams)
{
    auto paramsTmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_PARAMS, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (paramsTmp->isNull()) {
        return *paramsTmp;
    }

    return paramsTmp->toArray();
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, getException)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_EXCEPTION, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (tmp->isNull()) {
        return *tmp;
    }

    auto exception = tmp->toObject();
    if (exception->o_instanceof(String("Exception"))) {
        return exception;
    }

    return Variant(Variant::NullInit());
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, getModuleName)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (tmp->isNull()) {
        return *tmp;
    }

    return tmp->toString();
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, getControllerName)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_CONTROLLER, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (tmp->isNull()) {
        return *tmp;
    }

    return tmp->toString();
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, getActionName)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_ACTION, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (tmp->isNull()) {
        return *tmp;
    }

    return tmp->toString();
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, setModuleName, 
        const Variant& module)
{
    if (!module.isString()) {
        return false;
    }
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    *tmp = module;
    return true;
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, setControllerName, 
        const Variant& module)
{
    if (!module.isString()) {
        return false;
    }

    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_CONTROLLER, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    *tmp = module;
    return true;
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, setActionName, 
        const Variant& module)
{
    if (!module.isString()) {
        return false;
    }

    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_ACTION, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    *tmp = module;
    return true;
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, getMethod)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_METHOD, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    if (tmp->isNull()) {
        return *tmp;
    }

    return tmp->toString();
}

static Variant yaf_request_get_language()
{
    Variant lang = php_global(S_SERVER).toArray()[String("HTTP_ACCEPT_LANGUAGE")];
    if (!lang.isString()) {
        return lang;
    }

    char* str_lang = strdup(lang.toString().toCppString().c_str());
    std::string prefer;

    double max_qvalue = 0;
    char* save_ptr = NULL;

    char* one_ptr = strtok_r(str_lang, ",", &save_ptr);
    while (one_ptr) {
        char* qvalue = NULL;
        while (*one_ptr == ' ') one_ptr++;
        if ((qvalue = strstr(one_ptr, "q="))) {
            float qval = atof(qvalue + 2);
            if (qval > max_qvalue) {
                max_qvalue = qval;
                prefer = std::string(one_ptr, (qvalue - one_ptr - 1));
            }
        } else {
            if (max_qvalue < 1) {
                max_qvalue = 1;
                prefer = std::string(one_ptr, strlen(one_ptr));
            }
        }
        one_ptr = strtok_r(NULL, ",", &save_ptr);
    }
    
    free(str_lang);
    return String(prefer.c_str());
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, getLanguage)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_LANG, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (tmp->isString()) {
        return tmp->toString();
    }

    Variant lang = yaf_request_get_language();
    *tmp = lang;
    return lang;
}

static bool HHVM_METHOD(Yaf_Request_Abstract, setBaseUri, const Variant& uri)
{
    if (!uri.isString()) {
        return false;
    }

    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_BASE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    *tmp = uri;
    return true; 
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, getBaseUri)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_BASE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    if (tmp->isNull()) {
        return *tmp;
    }

    return tmp->toString();
}

static Variant HHVM_METHOD(Yaf_Request_Abstract, getRequestUri)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_URI, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    if (tmp->isNull()) {
        return *tmp;
    }

    return tmp->toString();
}

static bool HHVM_METHOD(Yaf_Request_Abstract, setRequestUri, const Variant& uri)
{
    if (!uri.isString()) {
        return false;
    }

    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_URI, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    *tmp = uri;
    return true; 
}

static bool HHVM_METHOD(Yaf_Request_Abstract, isDispatched)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_STATE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (tmp->isNull()) {
        return false;
    }

    return tmp->toBoolean(); 
}

static bool HHVM_METHOD(Yaf_Request_Abstract, setDispatched, const bool& dispathed)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_STATE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    *tmp = dispathed;
    return true; 
}

static bool HHVM_METHOD(Yaf_Request_Abstract, isRouted)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_ROUTED, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    if (tmp->isNull()) {
        return false;
    }

    return tmp->toBoolean(); 
}

static bool HHVM_METHOD(Yaf_Request_Abstract, setRouted, const bool& routed)
{
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_ROUTED, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");

    *tmp = routed;
    return true; 
}

void YafExtension::_initYafRequestClass()
{
    HHVM_ME(Yaf_Request_Abstract, isGet);
    HHVM_ME(Yaf_Request_Abstract, isPost);
    HHVM_ME(Yaf_Request_Abstract, isPut);
    HHVM_ME(Yaf_Request_Abstract, isHead);
    HHVM_ME(Yaf_Request_Abstract, isOptions);
    HHVM_ME(Yaf_Request_Abstract, isCli);
    HHVM_ME(Yaf_Request_Abstract, isXmlHttpRequest);
    HHVM_ME(Yaf_Request_Abstract, getServer);
    HHVM_ME(Yaf_Request_Abstract, getEnv);
    HHVM_ME(Yaf_Request_Abstract, setParam);
    HHVM_ME(Yaf_Request_Abstract, getParam);
    HHVM_ME(Yaf_Request_Abstract, getParams);
    HHVM_ME(Yaf_Request_Abstract, getException);
    HHVM_ME(Yaf_Request_Abstract, getModuleName);
    HHVM_ME(Yaf_Request_Abstract, getControllerName);
    HHVM_ME(Yaf_Request_Abstract, getActionName);
    HHVM_ME(Yaf_Request_Abstract, setModuleName);
    HHVM_ME(Yaf_Request_Abstract, setControllerName);
    HHVM_ME(Yaf_Request_Abstract, setActionName);
    HHVM_ME(Yaf_Request_Abstract, getMethod);
    HHVM_ME(Yaf_Request_Abstract, getLanguage);
    HHVM_ME(Yaf_Request_Abstract, setBaseUri);
    HHVM_ME(Yaf_Request_Abstract, getBaseUri);
    HHVM_ME(Yaf_Request_Abstract, getRequestUri);
    HHVM_ME(Yaf_Request_Abstract, setRequestUri);

    HHVM_ME(Yaf_Request_Abstract, isDispatched);
    HHVM_ME(Yaf_Request_Abstract, setDispatched);
    HHVM_ME(Yaf_Request_Abstract, isRouted);
    HHVM_ME(Yaf_Request_Abstract, setRouted);

    _initYafRequestHttpClass();
    _initYafRequestSimpleClass();
}

}
