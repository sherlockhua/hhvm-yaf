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


namespace HPHP{

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
    auto tmp = this_->o_realProp(YAF_REQUEST_PROPERTY_NAME_MODULE, 
            ObjectData::RealPropUnchecked, "Yaf_Request_Abstract");
    *tmp = module;
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

    _initYafRequestHttpClass();
    _initYafRequestSimpleClass();
}

}
