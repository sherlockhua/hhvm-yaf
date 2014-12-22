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
    return false;
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

    _initYafRequestHttpClass();
    _initYafRequestSimpleClass();
}

}
