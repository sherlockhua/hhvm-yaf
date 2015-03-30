/*===========================================
*   Copyright (C) 2015 All rights reserved.
*   
*   company      : xiaomi
*   author       : sherlockhua
*   email        : sherlockhua@xiaomi.com
*   date         ：2015-02-02 12:06:19
*   description  ：
*
=============================================*/
#include "yaf_router_supervar.h"
#include "yaf_router_static.h"
#include "hphp/runtime/base/php-globals.h"
#include "yaf_application.h"
#include "ext_yaf.h"
#include "yaf_request.h"


namespace HPHP {

Variant yaf_route_supervar_instance(const Object* object, const Variant& name)
{
    if (!name.isString() || 
            name.isString() && name.toString().length() == 0) {
        return init_null_variant;
    }

    Object o;
    if (object) {
        o = *object;
    } else {
        Array params = Array::Create();
        params.append(name);

        o = createObject("Yaf_Route_Supervar", params);
    }

    auto ptr_supervar = o->o_realProp(YAF_ROUTE_SUPERVAR_PROPETY_NAME_VAR, 
                ObjectData::RealPropUnchecked, "Yaf_Route_Supervar");

    *ptr_supervar = name;
    return o;
}

static Variant yaf_route_supervar_route(const Object& o, const Object& request)
{
    auto ptr_supervar = o->o_realProp(YAF_ROUTE_SUPERVAR_PROPETY_NAME_VAR, 
                ObjectData::RealPropUnchecked, "Yaf_Route_Supervar");

    Variant uri;
    if (php_global(S_GET).toArray().exists(ptr_supervar->toString())) {
        uri = php_global(S_GET).toArray()[ptr_supervar->toString()];
    }

    if (uri.isNull()) {
        return false;
    }

    yaf_route_pathinfo_route(request, uri.toString().c_str(), uri.toString().length());
    return true;
}

static void HHVM_METHOD(Yaf_Route_Supervar, __construct, const Variant& supervar)
{
    if (!supervar.isString() || supervar.toString().length() == 0) {
        yaf_trigger_error(YAF_ERR_TYPE_ERROR, 
                "Yaf_Route_Supervar, Expects a valid string super var name");
        return;
    }

    auto ptr_supervar = this_->o_realProp(YAF_ROUTE_SUPERVAR_PROPETY_NAME_VAR, 
                ObjectData::RealPropUnchecked, "Yaf_Route_Supervar");
    *ptr_supervar = supervar;
}

static Variant HHVM_METHOD(Yaf_Route_Supervar, route, const Variant& request)
{
    if (!request.isObject()) {
        return false;
    }

    if (!request.toObject()->o_instanceof("Yaf_Request_Abstract")) {
        return false;
    }

    return yaf_route_supervar_route(this_, request.toObject());
}

void YafExtension::_initYafSupervarRouterClass()
{
    HHVM_ME(Yaf_Route_Supervar, __construct);
    HHVM_ME(Yaf_Route_Supervar, route);
}



}
